#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
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
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string line;
  std::string key;
  float memValue;
  std::string memSize;
  float totalMemory{0.00};
  float avlblMemory{0.00};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> memValue >> memSize) {
        if (key == "MemTotal:") {
          totalMemory = memValue;
        } else if (key == "MemAvailable:") {
          avlblMemory = memValue;
        }
      }
    }
  }
  return 1 - (avlblMemory / totalMemory);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string upTime, idleTime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return stol(upTime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::string line;
  std::string cpu{"cpu"};
  CPUStates states{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> states.kUser_ >> states.kNice_ >> states.kSystem_ >>
        states.kIdle_ >> states.kIOwait_ >> states.kIRQ_ >> states.kSoftIRQ_;
  }
  return states.Sum();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string tok;
  string ut, st, ct, cst;
  long totalTime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 1; i < 14; i++) {
        linestream >> tok;
      }
      linestream >> ut >> st >> ct >> cst;
      totalTime = stol(ut) + stol(st) + stol(ct) + stol(cst);
    }
  }
  return totalTime;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::string line;
  std::string cpu;
  CPUStates states{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> states.kUser_ >> states.kNice_ >> states.kSystem_ >>
        states.kIdle_ >> states.kIOwait_ >> states.kIRQ_ >> states.kSoftIRQ_;
  }

  return states.kIdle_ + states.kIOwait_;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::string line;
  std::string cpu;
  CPUStates states{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> states.kUser_ >> states.kNice_ >> states.kSystem_ >>
        states.kIdle_ >> states.kIOwait_ >> states.kIRQ_ >> states.kSoftIRQ_;
  }
  return states.Sum() - (states.kIdle_ + states.kIOwait_);
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() {
  return (float)ActiveJiffies() / (float)Jiffies();
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line;
  std::string str;
  std::string str_int;
  int totalProcs{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> str >> str_int) {
        if (str == "processes") totalProcs = std::stoi(str_int);
      }
    }
  }
  return totalProcs;
}
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string runningProcs;
  int val{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> runningProcs) {
        if (key == "procs_running") val = std::stoi(runningProcs);
      }
    }
  }
  return val;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string line{"NONE"};
  PIDStates states{};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string line;
  std::string key, value;
  std::string ramValue;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") ramValue = std::to_string(stoi(value) / 1024);
      }
    }
  }
  return ramValue;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::string line;
  std::string userID{"NONE"};
  std::string token_str;
  auto token_auto{1};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token_str >> token_auto) {
        if (token_str == "Uid:") userID = std::to_string(token_auto);
      }
    }
  }
  return userID;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string line;
  std::string user, x, uid_lhs;
  std::string uid_rhs{LinuxParser::Uid(pid)};
  std::ifstream filestream(kPasswordPath);
  std::string value{"NONE"};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid_lhs) {
        if (uid_lhs == uid_rhs) value = user;
      }
    }
  }
  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string var;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 1; i < 23; i++) linestream >> var;  // 22nd value
      return stol(var) / sysconf(_SC_CLK_TCK);
    }
  }
  return -1;
}
