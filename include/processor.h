#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"
#include <vector>

class Processor {
 public:
  float Utilization() const;  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float cpu_utilization{LinuxParser::CpuUtilization()};
};

#endif
