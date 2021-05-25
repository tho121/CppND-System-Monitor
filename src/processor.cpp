#include "processor.h"
#include "linux_parser.h"

// aggregate CPU utilization
float Processor::Utilization() { 
    
    long activeJiffies = LinuxParser::ActiveJiffies();
    long idleJiffies = LinuxParser::IdleJiffies();

    long total = (activeJiffies + idleJiffies) - (prevActiveJiffies_ + prevIdleJiffies_);

    double active = static_cast<double>(total - (idleJiffies - prevIdleJiffies_));

    float utilization = static_cast<float>(active / static_cast<double>(total));

    prevActiveJiffies_ = activeJiffies;
    prevIdleJiffies_ = idleJiffies;

    return utilization; 
}