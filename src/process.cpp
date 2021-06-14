#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
: pid_(pid)
, user_(LinuxParser::User(pid))
, command_(LinuxParser::Command(pid))
{
    lastTick_ = UpTime();
}

int Process::Pid() const { return pid_; }

void Process::Update(long currentUptime, long totalJiffies)
{
    if(lastTick_ < currentUptime)
    {
        //update cpu utilization
        long activeJiffies = LinuxParser::ActiveJiffies(pid_);
        double process_time = static_cast<double>(activeJiffies - prevActiveJiffies_);
        double total_time = static_cast<double>(totalJiffies - prevTotalJiffies_);
    
        prevActiveJiffies_ = activeJiffies;
        prevTotalJiffies_ = totalJiffies;

        cpuUtilization_ = static_cast<float>(process_time / total_time);

        //update ram and uptime
        ram_ = LinuxParser::Ram(pid_);
        upTime_ = currentUptime - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
        
        lastTick_ = currentUptime;
    }
}

// Return this process's CPU utilization
float Process::CpuUtilization() const {return cpuUtilization_;}

// Return the command that generated this process
string Process::Command() const { return command_; }

// Return this process's memory utilization
string Process::Ram() const { return ram_; }

// Return the user (name) that generated this process
string Process::User() const { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() const {return upTime_;}

bool Process::operator<(Process const& a) const { 

    auto otherUtilization = a.CpuUtilization();
    if(cpuUtilization_ < otherUtilization)
        return true;
    if(cpuUtilization_ > otherUtilization)
        return false;

    auto otherRam = a.Ram();
    if(ram_ < otherRam)
        return true;
    if(ram_ > otherRam)
        return false;

    auto otherUptime = a.UpTime();
    if(upTime_ < otherUptime)
        return true;
    if(upTime_ > otherUptime)
        return false;

    return false;
}