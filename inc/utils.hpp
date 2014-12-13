#ifndef RADIO_EMISSION_UTILS_HPP
#define RADIO_EMISSION_UTILS_HPP

// STD includes
#include <string>

namespace utils {
  void log(const std::string& a);
  
  int scheduler_realtime();
  int scheduler_standard();
} //endof namespace utils

#endif // RADIO_EMISSION_UTILS_HPP
  
