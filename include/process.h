#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // Done: See src/process.cpp
  std::string User();                      // Done: See src/process.cpp
  std::string Command();                   // Done: See src/process.cpp
  float CpuUtilization();                  // Done: See src/process.cpp
  std::string Ram();                       // Done: See src/process.cpp
  long int UpTime();                       // Done See src/process.cpp
  bool operator<(Process const& a) const;  // Done: See src/process.cpp

  void Pid(int pid) { pid_ = pid; }
  Process(int pid){ pid_ = pid;}

  // Done: Declare any necessary private members
 private:
    int pid_;
};


#endif