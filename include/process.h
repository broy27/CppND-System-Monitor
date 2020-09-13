#ifndef PROCESS_H
#define PROCESS_H

#include <linux_parser.h>
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  explicit Process(int pid) : pid_(pid) {}
  int Pid() const { return pid_; }
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;
  // TODO: Declare any necessary private members
 private:
  int pid_{0};
};

#endif
