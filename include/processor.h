#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
    long prevIdleJiffies_{0};
    long prevActiveJiffies_{0};
};

#endif