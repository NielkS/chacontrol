/*  @Date  	2014/10/11  
 *  @Author N.Klein
 */


// STD includes
#include <cassert>
#include <stdlib.h> ///< atoi
#include <iostream>	///< std::cin, std::cout

// project includes
#include "chacon.hpp"
#include "utils.hpp"
#include "wiring_pi.hpp"

int main (int argc, char** argv)
{
	//utils::log("Demarrage du programme");

  if ( argc == 5 )
  { // straightforward mode
    const unsigned pin          = atoi(argv[1]);
	  const unsigned recipient    = atoi(argv[2]);
  	const unsigned interruptor  = atoi(argv[3]);
    const chacon::eState target = (std::string(argv[4]) == "on" ? chacon::ON : chacon::OFF);
    
	  chacon::socket sock(pin);
  
    const chacon::command cmd(recipient, interruptor, target);  	
    std::cout << cmd << std::endl;
    sock.transmit(cmd);
  } else { // interactive mode
    unsigned pin;
    unsigned recipient;
    std::string target;
    
    std::cout << "Select GPIO pin to be used" << std::endl;
    std::cin  >> pin;
  
    chacon::socket sock(pin);
    while ( true ) {
      std::cout << "Recipient" << std::endl;
      std::cin  >> recipient;
      
      std::cout << "State ('ON' / 'OFF' ), or ENTER to quit" << std::endl;
      std::cin.ignore();
      std::getline(std::cin, target);
			
			if ( target == "")
				break;
      
      const chacon::command cmd(recipient, 0, target == "ON" ? chacon::ON : chacon::OFF);
      std::cout << cmd << std::endl;
      sock.transmit(cmd);
    }
  }
  
  //utils::log("fin du programme");    // execution terminée.
  return 0;
}
  
