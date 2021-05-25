#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    
    vector<int> pids = LinuxParser::Pids();

    set<int> pid_set;

    for(auto& process : processes_)
    {
        pid_set.insert(process.Pid());
    }

    for(auto& pid : pids)
    {
        auto it = pid_set.find(pid);
        if(it == pid_set.end())
        {
            processes_.emplace_back(Process(pid));
        }
    }

    long currentUptime = UpTime();
    long totalJiffies = LinuxParser::Jiffies();

    //update cpu utilization before sorting and returning
    for(auto& process : processes_)
    {
        process.Update(currentUptime, totalJiffies);
    }

    //highest cpu utilization first
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(), processes_.end());

    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }