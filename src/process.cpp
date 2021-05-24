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
//, uptimeTick_(LinuxParser::UpTime(pid))
{
    uptimeTick_ = UpTime();

}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

void Process::Update(long currentUptime, long totalJiffies)
{
    long uptime = currentUptime;

    if(uptimeTick_ < currentUptime)
    {
        long activeJiffies = LinuxParser::ActiveJiffies(pid_);
        double process_time = static_cast<double>(activeJiffies - prevActiveJiffies_);
        double total_time = static_cast<double>(totalJiffies - prevTotalJiffies_);
    
        prevActiveJiffies_ = activeJiffies;
        prevTotalJiffies_ = totalJiffies;

        cpuUtilization_ = static_cast<float>(process_time / total_time);

        ram_ = LinuxParser::Ram(pid_);
        upTime_ = LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
        
        uptimeTick_ = currentUptime;
    }
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {return cpuUtilization_;}

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const {return upTime_;}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
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