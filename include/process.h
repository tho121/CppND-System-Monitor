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

    int Pid() const;                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization(bool doUpdate);                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp
    float LastCpuUtilization() const;

  // TODO: Declare any necessary private members
 private:
    int pid_{-1};
    std::string user_;
    std::string command_;
    long prevActiveJiffies_{0};
    long prevTotalJiffies_{0};
    float cpuUtilization_{0};
    long uptimeTick_{0};
};

#endif