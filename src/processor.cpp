#include "processor.h"
#include "linux_parser.h"
#include <vector>



// Return the aggregate CPU utilization
// formula used from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
  std::vector<int> newStates = LinuxParser::CpuUtilization();
  
  int idle = states[LinuxParser::CPUStates::idle] + states[LinuxParser::CPUStates::ioWait];
  int newIdle = newStates[LinuxParser::CPUStates::idle] + newStates[LinuxParser::CPUStates::ioWait];
  
  int nonIdle = (states[LinuxParser::CPUStates::user] + 
                 states[LinuxParser::CPUStates::nice] +  
                 states[LinuxParser::CPUStates::system] + 
                 states[LinuxParser::CPUStates::irq] + 
                 states[LinuxParser::CPUStates::softIrq] + 
                 states[LinuxParser::CPUStates::steal]);

  int newNonIdle = (newStates[LinuxParser::CPUStates::user] +
                    newStates[LinuxParser::CPUStates::nice] +
                    newStates[LinuxParser::CPUStates::system] +
                    newStates[LinuxParser::CPUStates::irq] + 
                    newStates[LinuxParser::CPUStates::softIrq] + 
                    newStates[LinuxParser::CPUStates::steal]);
  
	int total = idle + nonIdle;
	int newTotal = newIdle + newNonIdle;

	float totald = newTotal - total;
	float idled = newIdle - idle;

	float utilization = (float(totald) - float(idled)) / float(totald);
  	states = newStates;
  	return utilization; 
}