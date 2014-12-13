#include "utils.hpp"

// STD includes
/*
#include <cassert>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
*/
#include <iostream>///< std::cerr, std::cout
#include <sched.h> ///< sched_param, sched_setscheduler, sched_get_priority_max
#include <string>	 ///< std::string

// NAMESPACE UTILS
void utils::log(const std::string& a) {
  std::cout << a << std::endl;
}

int utils::scheduler_realtime() {
  struct sched_param p;
  p.__sched_priority = sched_get_priority_max(SCHED_RR);
  const int err_code = sched_setscheduler( 0, SCHED_RR, &p );
  if( err_code == -1) {
    std::cerr << "Failed to switch to realtime scheduler." << std::endl;
  }
  return err_code;
}
  
int utils::scheduler_standard() {
  struct sched_param p;
  p.__sched_priority = 0;
  const int err_code = sched_setscheduler( 0, SCHED_OTHER, &p );
  if( err_code == -1 ) {
    std::cerr << "Failed to switch to normal scheduler." << std::endl;
  }
  return err_code;
} 
