#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

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
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> os >> kernel;
  }
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

vector<string> parse_values(const string& line){
  std::istringstream iss(line);
  std::istream_iterator<string> beg(iss), end;
  vector<string> values(beg, end);
  return values;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, dump1, dump2;
  float ram, free_memory, total_memory;
  vector<float> memory;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> dump1 >> ram >> dump2;

    if ((dump1.compare("Buffers:") == 0))
      break;  // Exit the while when reaching Buffers

    memory.emplace_back(ram);
  }
  stream.close();

  free_memory = memory.back();
  memory.pop_back();
  memory.pop_back();
  total_memory = memory.back();
  memory.pop_back();

  return ((total_memory - free_memory) / total_memory);
}

// Done Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory+kUptimeFilename);

  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }

  stream.close();
  return uptime;
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    vector<string> jiffs = LinuxParser::CpuUtilization();
    float sum = 0;
    for(string j : jiffs){
      sum += std::stof(j);
    }
    return sum;
}

// Done: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 

  string utime, stime, cutime, cstime, line, value;
  const string directory = kProcDirectory + std::to_string(pid) + kStatFilename;
  std::ifstream filestream(directory);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 0; i < 13; i++) {
        linestream >> value;
      }
      linestream >> utime >> stime >> cutime >> cstime; 
      float total_time = std::stof(utime) + std::stof(stime) + std::stof(cutime) + std::stof(cstime);
      return total_time;
    }
  }
  filestream.close();
  return 0;
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    vector<string> jiffs = LinuxParser::CpuUtilization();
    float sum = 0;
    jiffs.erase(jiffs.begin() + 3);
    for (string j : jiffs){
      sum+= std::stof(j);
    }
    return sum;
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffs = LinuxParser::CpuUtilization();
  return std::stof(jiffs[3]); 
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  const string directory = kProcDirectory + kStatFilename;
  std::ifstream filestream(directory);
  string line;
  vector<string> vals;

  getline(filestream, line);
  std::istringstream iss(line);
  std::istream_iterator<string> beg(iss), end;

  vector<string> values(beg, end);

  for (std::size_t i = 1; i < values.size(); i++){
    vals.push_back(values[i]);
  }  
  
  return vals; 
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, proc;
  int value;

  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> proc >> value;

    if ((proc.compare("processes") == 0)) {
      stream.close();
      return value;
    }
  }
  return 0;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, proc;
  int value;

  std::ifstream stream(kProcDirectory + kStatFilename);

  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> proc >> value;

    if ((proc.compare("procs_running") == 0)) {
      stream.close();
      return value;
    }
  }
  return 0; 
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::string command, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
    stream.close();
    return command;
  }

  return "CMD Not Found";
}

// Done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::string code, vmSize, idle, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    do {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> code >> vmSize >> idle;
      if (stream.eof()) {
        return "0";
      }
    } while (code.compare("VmSize:") != 0);
    stream.close();
    return (std::to_string(stoi(vmSize) / 1024));
  }
  return "0";
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string code, uid, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    do {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> code >> uid;
    } while (code.compare("Uid:") != 0);
    stream.close();
    return uid;
  }
  return 0;

}

// Done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string user, userid, dump, line;
  std::ifstream stream(kPasswordPath);
  string uid = LinuxParser::Uid(pid);

  if (stream.is_open()) {
    do {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> dump >> userid;
      if (stream.eof()) return "User not found";
    } while (userid.compare(uid) != 0);
    stream.close();
    return user;
  }
  return "0";

}

// Done: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  const string directory = kProcDirectory + to_string(pid) + kStatFilename;
  string value;
  std::ifstream filestream(directory);
  if (filestream.is_open()){
    for (int i = 0; i < 22; ++i) filestream >> value;
      filestream.close();
      return LinuxParser::UpTime() - std::stol(value) / sysconf(_SC_CLK_TCK);
  }
  return 0;
}