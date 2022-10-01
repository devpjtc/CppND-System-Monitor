#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;


string Format::FormatNumber(long number, int length) {
  std::stringstream ss;
  ss << std::setw(length) << std::setfill('0') << number;
  return ss.str();
}

// helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long time) { 
  int s = time;
  int h, m;
  h = s / 3600;
  s -= h * 3600;
  m = s / 60;
  s -= m * 60;
  return FormatNumber(h, 2) + ":" + FormatNumber(m, 2) + ":" + FormatNumber(s, 2);
  
}