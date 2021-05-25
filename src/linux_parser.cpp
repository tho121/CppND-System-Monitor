#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"


using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  int value;
  string scale;

  int totalMemSize = 0;
  int freeMemSize = 0;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> scale) {
        if (key == "MemTotal:") {
          totalMemSize = value;
          continue;
        }

        if (key == "MemFree:") {
          freeMemSize = value;
          break;  //break loop, no need to continue parsing
        }
      }
    }
  }

  return static_cast<float>(totalMemSize - freeMemSize) / static_cast<float>(totalMemSize); 
}

long LinuxParser::UpTime() { 
  double uptime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idleTime;
  }

  return static_cast<long>(uptime); 
}

long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

long LinuxParser::ActiveJiffies(int pid) { 
  
  string line;
  string value;
  long utime;
  long stime;
  long cutime;
  long cstime;
  long starttime;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      for (int i = 1; i < 23; ++i) {
        linestream >> value;

        if(i == 14)
        {
          utime = std::stol(value);
        }
        else if(i == 15)
        {
          stime = std::stol(value);
        }
        else if(i == 16)
        {
          cutime = std::stol(value);
        }
        else if(i == 17)
        {
          cstime = std::stol(value);
        }
        else if(i == 22)
        {
          starttime = std::stol(value);
          
          //last number we need, early out
          return utime + stime + cutime + cstime + starttime;
        }
      }
    }
  }  

  return 0; 
  
}

long LinuxParser::ActiveJiffies() { 
  
  string line;
  string key;

  long user = 0;
  long nice = 0;
  long system = 0;
  long idle = 0;
  long iowait = 0;
  long irq = 0;
  long softirq = 0;
  long steal = 0;
  long guest = 0;
  long guest_nice = 0;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
        if (key == "cpu") {
          return user + nice + system + irq + softirq + steal;
        }
      }
    }
  }

  return 0;
}

long LinuxParser::IdleJiffies() { 

  string line;
  string key;

  long user = 0;
  long nice = 0;
  long system = 0;
  long idle = 0;
  long iowait = 0;
  long irq = 0;
  long softirq = 0;
  long steal = 0;
  long guest = 0;
  long guest_nice = 0;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
        if (key == "cpu") {
          return idle + iowait;
        }
      }
    }
  }

  return 0; 
}

int LinuxParser::TotalProcesses() {

  string line;
  string key;
  int value;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }

  return 0; 
}

int LinuxParser::RunningProcesses() { 

  string line;
  string key;
  int value;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }

  return 0; 
}

string LinuxParser::Command(int pid) { 
  
  string line;
  string cmdline;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cmdline) {
        return cmdline;
      }
    }
  }

  return string(); 
}

string LinuxParser::Ram(int pid) { 
  
  string line;
  string key;
  long value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return std::to_string(value / 1000);
        }
      }
    }
  }
  return string(); 
  
}

string LinuxParser::Uid(int pid) { 

  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }

  return string(); 
}

string LinuxParser::User(int pid) { 
  
  string line;
  string user;
  string value;
  string uid = Uid(pid);
  char c;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while (linestream >> user >> c >> value) {
        if (uid == value) {
          return user;
        }
      }
    }
  }
  
  return string(); 
}

long LinuxParser::UpTime(int pid) { 

  string line;
  string value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      //the 22nd value is starttime in clockticks
      for (int i = 1; i < 23; ++i) {
        linestream >> value;

        if(i == 22)
        {
          return std::stol(value);
        }
      }
    }
  }

  return 0; 
}
