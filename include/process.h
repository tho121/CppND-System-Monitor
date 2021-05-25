#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
    Process(int pid);

    int Pid() const;                               
    std::string User() const;                      
    std::string Command() const;                   
    float CpuUtilization() const;                  
    std::string Ram() const;                       
    long int UpTime() const;                       
    bool operator<(Process const& a) const; 
    void Update(long currentUptime, long totalJiffies = 0);

 private:
    int pid_{-1};
    std::string user_;
    std::string command_;
    long prevActiveJiffies_{0};
    long prevTotalJiffies_{0};
    float cpuUtilization_{0};
    std::string ram_{""};
    long int upTime_{0};
    long lastTick_{0};
};

#endif