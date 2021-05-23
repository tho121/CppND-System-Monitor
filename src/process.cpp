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

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    
    long uptime = LinuxParser::UpTime();

    if(uptimeTick_ < uptime)
    {
        long total_time = LinuxParser::ActiveJiffies(pid_);
    
        double seconds = static_cast<double>(uptime - UpTime());
        double percentage = static_cast<double>(total_time / sysconf(_SC_CLK_TCK)) / seconds;

        cpuUtilization_ = static_cast<float>(percentage);
    }

    return cpuUtilization_;   
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

float Process::LastCpuUtilization() const {return cpuUtilization_;}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpuUtilization_ < a.LastCpuUtilization();  
}