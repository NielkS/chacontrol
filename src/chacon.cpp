#include "chacon.hpp"

// STD includes
#include <cassert>	///< assert
#include <stdexcept>///< std::exception
#include <stdio.h>	///< perror
#include <string>		///< std::string
#include <unistd.h> ///< setuid

// project includes
#include "wiring_pi.hpp"
#include "utils.hpp"

// declaration of helper;
void send_bit_raw_(unsigned pin, bool b);


// implementation
  
// Send pulse, i.e., high-to-low edge.
// 1 = 310µs haut puis 1340µs bas
// 0 = 310µs haut puis 310µs bas
void send_bit_raw_(unsigned pin, bool b) {
  wiring_pi::write_bit(pin, HIGH, chacon::DURATION_HIGH_BIT_ANY);
  wiring_pi::write_bit(pin, LOW , b ? chacon::DURATION_LOW_BIT_TRUE : chacon::DURATION_LOW_BIT_FALSE);
}
  
// NAMESPACE CHACON

// Send two pulses for each logical bit : 
//    logical 0 equals '01', and
//    logical 1 equals '10'
// Encoding presumably chosen for robustness sake...
void chacon::send_bit(unsigned pin, bool b) {
  send_bit_raw_(pin, b);
  send_bit_raw_(pin, not b);
}

void chacon::connect(unsigned pin) {
  wiring_pi::write_bit(pin, HIGH, DURATION_HIGH_BIT_ANY);// Noise bit resets receptors timer
  wiring_pi::write_bit(pin, LOW , 9900);                 // First lock : 9900µs
  wiring_pi::write_bit(pin, HIGH, DURATION_HIGH_BIT_ANY);
  wiring_pi::write_bit(pin, LOW , 2675);  // Second lock : 2675µs
  wiring_pi::write_bit(pin, HIGH);       // Finish on high state to lock data tx
}

void chacon::disconnect(unsigned pin) {
  wiring_pi::write_bit(pin, HIGH, DURATION_HIGH_BIT_ANY);   // Lock data
  wiring_pi::write_bit(pin, LOW , 2675);  // long sleep in low state means end-of-signal
}
  
void chacon::transmit(unsigned pin, const command& cmd) {
  chacon::connect(pin);
  
  // Send emitter code, MSB first
  for(int i=0; i<26;i++) { 
    chacon::send_bit(pin, cmd.recipient_id & (1 << i) ); 
  }
  
  // Send is_group bit
  chacon::send_bit(pin, cmd.is_group_cmd);
  
  // Send bit for target state
  chacon::send_bit(pin, cmd.target == ON);
  
  // Send interruptor code, MSB first.
  // nb: on official remote controllers, interruptors are 0-based, i.e., id ranges from 0 to 3
  // interrupteur 1 = 0 (donc 0000) , interrupteur 2 = 1 (1000) , interrupteur 3 = 2 (0100) etc...
  for(int i=0; i<4;i++) {
    chacon::send_bit(pin, cmd.interrupt_id & (1 << i) ); 
  }
  
  chacon::disconnect(pin);
}

std::ostream& chacon::operator<<(std::ostream& os, eState state) {
  switch (state) {
    case (ON)  : { os << "ON";  break; }
    case (OFF) : { os << "OFF"; break; }
    default    : { throw std::runtime_error("fixme!"); }
  }
  return os;
}

std::ostream& chacon::operator<<(std::ostream& os, const command& cmd) {
  os 	<< "Set device " 	<< cmd.recipient_id 
  		<< " " 						<< cmd.target
  		<< " with IR " 		<< cmd.interrupt_id ;
  return os;
}

void chacon::socket::s_init() {
  if ( 0 == s_instances++ ) {
    int error_code(0);
	  if (setuid(0) )	{
		  perror("setuid");
		  error_code = ERR_SETUID;
	  } else {
	 	  utils::scheduler_realtime();
		  if( wiring_pi::init() == -1) {
		    utils::log("Cannot link with libWiringPI");
		    error_code = ERR_LIBWIRING;
		  }
    }
    if ( error_code ) {
      throw std::runtime_error( std::string("Failed to setup device - error ") + STR_ERROR[error_code] );  
    }
  }
}

unsigned chacon::socket::s_instances = 0;

void chacon::socket::s_destroy() {
  if ( 0 == --s_instances )
		utils::scheduler_standard();
}
  
chacon::socket::socket(unsigned pin) 
  : m_pin(pin)
{ 
  s_init();
  wiring_pi::set_mode(m_pin,OUTPUT);
  utils::log("GPIO pin configured as DO.");
}

chacon::socket::~socket() {
  s_destroy();
}

void chacon::socket::transmit(const command& cmd, unsigned count) {
  for (unsigned i=0; i < count; ++i) {
    chacon::transmit(m_pin, cmd);
    wiring_pi::sleep_for(10 * wiring_pi::millisecond);    
  }
}

