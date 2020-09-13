#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
struct CPUStates {
  long kUser_;
  long kNice_;
  long kSystem_;
  long kIdle_;
  long kIOwait_;
  long kIRQ_;
  long kSoftIRQ_;
  long kSteal_;

  [[nodiscard]] long Sum() const {
    return kUser_ + kNice_ + kSystem_ + kIdle_ + kIOwait_ + kIRQ_ + kSoftIRQ_;
  }
};

struct PIDStates {
  int pid_;
  std::string exName;
  char state;
  unsigned euid, egid;
  int ppid;
  int pgrp;
  int session;
  int tty;
  int tpgid;
  unsigned int flags;
  unsigned int minflt;
  unsigned int cminflt;
  unsigned int majflt;
  unsigned int cmajflt;
  int utime;  // 16
  int stime;  // 17
  int cutime;
  int cstime;
  int counter;
  int priority;
  unsigned int timeout;
  unsigned int itrealvalue;
  int starttime;
  unsigned int vsize;
  unsigned int rss;
  unsigned int rlim;
  unsigned int startcode;
  unsigned int endcode;
  unsigned int startstack;
  unsigned int kstkesp;
  unsigned int kstkeip;
  int signal;
  int blocked;
  int sigignore;
  int sigcatch;
  unsigned int wchan;
  int sched, sched_priority;
};

float CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif
