#ifndef RADIO_EMISSION_WIRING_PI_HPP
#define RADIO_EMISSION_WIRING_PI_HPP

// third-party
#include <wiringPi/wiringPi.h>

// wrap external lib calls in namespaces for sake of readability. 
namespace wiring_pi {
  static const unsigned millisecond(1000);
  
  inline int init() {
    return wiringPiSetup();
  }
  
  inline void sleep_for (unsigned usec) { 
    if (usec)
      delayMicroseconds(usec); 
  }

  inline void write_bit(unsigned pin, unsigned value, unsigned usec = 0) { 
    digitalWrite(pin, value);
    sleep_for(usec);
  }

  inline void set_mode(unsigned pin, int mode) {
    pinMode(pin, mode);
  }
} // endof namespace wiring_pi

#endif // RADIO_EMISSION_WIRING_PI_HPP
