#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int p) {
	pid = p;
	ram = LinuxParser::Ram(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
// formula used from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() {
  int uptime = LinuxParser::UpTime();
  vector<int> cpu_values = LinuxParser::CpuUtilization(pid);
  long utime, stime, cutime, cstime, starttime, totalTime, seconds;
  utime = cpu_values[0];
  stime = cpu_values[1];
  cutime = cpu_values[2];
  cstime = cpu_values[3];
  starttime = cpu_values[4];
  long hertz = sysconf(_SC_CLK_TCK);
  totalTime = utime + stime + cutime + cstime;
  seconds = uptime - (starttime / hertz);
  return (float(totalTime) / float(hertz)) / float(seconds); 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
	vector<int> cpu_values = LinuxParser::CpuUtilization(pid);
  	long starttime = cpu_values[4];
  	return starttime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process & a) {
	return stol(ram) > stol(a.ram);
}