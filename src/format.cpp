#include <stdio.h>
#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  char timeString[10];
  int hH = seconds / 3600;
  int rem = seconds % 3600;
  int mM = rem / 60;
  int sS = rem % 60;
  sprintf(timeString, "%.2d:%.2d:%.2d", hH, mM, sS);
  return timeString;
}
