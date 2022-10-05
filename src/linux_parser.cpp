#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          break;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
      // Is this a directory?
      if (file->d_type == DT_DIR) {
        // Is every character of the name a digit?
        string filename(file->d_name);
        if (std::all_of(filename.begin(), filename.end(), isdigit)) {
          int pid = stoi(filename);
          pids.push_back(pid);
        }
      }
    }
    closedir(directory);
    return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
	float memoryTotal, memoryFree;
  	string line, _;
  	std::ifstream stream(kProcDirectory + kMeminfoFilename);
  	if (stream.is_open()) {
    	std::getline(stream, line);
    	std::istringstream linestreamT(line);
    	linestreamT >> _ >> memoryTotal >> _;
      	std::getline(stream, line);
    	std::istringstream linestreamF(line);
    	linestreamF >> _ >> memoryFree >> _ ;
  }
  stream.close();
  return (memoryTotal - memoryFree) / memoryTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
	long uptimeSec;
  	string line;
  	std::ifstream stream(kProcDirectory + kUptimeFilename);
  	if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> uptimeSec;
    }
  	stream.close();
  	return uptimeSec;
}


vector<int> LinuxParser::CpuUtilization(int pid) {
  	std::vector<int> cpu_values = {};
	string line;
  	string value;
	std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  	std::istringstream linestream;
  	if (stream.is_open()) {
    	std::getline(stream, line);
      	linestream.str(line);
    	for (int i = 1; i < 23; i++) {
          	linestream >> value;
        	if ((i > 13 and i < 18) || (i == 22)) {
            	cpu_values.push_back(stol(value));
            }
        }
  	}
  	stream.close();
  	return cpu_values;
}


// TODO: Read and return CPU utilization
vector<int> LinuxParser::CpuUtilization() {
  string line, _;
  int value;
  vector<int> states = {};
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::istringstream linestream;
  if (stream.is_open()) {
  	std::getline(stream, line);
    linestream.str(line);
    linestream >> _ ;
    while (linestream >> value)
      states.push_back(value);
  }
  stream.close();
  return states; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
	int totalProcesses = 0;
  	string line, key;
  	std::ifstream stream(kProcDirectory + kStatFilename);
    std::istringstream linestream;
  	while (stream.is_open()) {
    	std::getline(stream, line);
      	linestream.str(line);
    	linestream >> key;
      	if (key == "processes") { 
          linestream >> totalProcesses; 
          break;
        }
  	}
  	stream.close();
  	return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
	int runningProcesses = 0;
  	string line, key;
  	std::ifstream stream(kProcDirectory + kStatFilename);
    std::istringstream linestream;
  	while (stream.is_open()) {
    	std::getline(stream, line);
      	linestream.str(line);
    	linestream >> key;
      	if (key == "procs_running") { 
          linestream >> runningProcesses; 
          break;
        }
  	}
  	stream.close();
  	return runningProcesses;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  	string line;
	std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  	if (stream.is_open()) {
    	std::getline(stream, line);
  	}
  	stream.close();
  	return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
	string line, key, ram="0";
	std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  	std::istringstream linestream;
  	while (std::getline(stream, line)) {
      	linestream.str(line);
    	linestream >> key;
      	if (key == "VmSize:") { 
          linestream >> ram; 
          ram = std::to_string(stol(ram) / 1000);
          break;
        }
  	}
  	stream.close();
  	return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  	string line, key, uid;
	std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  	std::istringstream linestream;
  	while (stream.is_open()) {
    	std::getline(stream, line);
      	linestream.str(line);
    	linestream >> key;
      	if (key == "Uid:") { 
          linestream >> uid; 
          break;
        }
  	}
  	stream.close();
  	return uid;;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
	string uid = Uid(pid);
	char delimeter = ':';
  	string line, user, _, in_uid;
  	std::ifstream stream(kPasswordPath);
  	std::istringstream linestream;
  	while (stream.is_open()) {
      	std::getline(stream, line);
      	linestream.str(line);

		std::getline(linestream, user, delimeter);
      	std::getline(linestream, _, delimeter);
      	std::getline(linestream, in_uid, delimeter);
      	if (in_uid == uid) { 
          break;
        }
  	}
  	stream.close();
  	return user;
  
}
