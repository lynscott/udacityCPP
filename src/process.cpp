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

// Done: Return this process's ID
int Process::Pid() { return pid_; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() {  return LinuxParser::ActiveJiffies(pid_); }

// Done: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Process::Pid()); }

// Done: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::Pid()); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Process::Pid()); }

// Done: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return (this->pid_ > a.pid_ ? true : false);
}