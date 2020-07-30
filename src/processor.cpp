#include "processor.h"
#include "linux_parser.h"

// Done: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float active = LinuxParser::ActiveJiffies();
    float jiffies = LinuxParser::Jiffies();
    return active / jiffies;
 }