#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  Processor() { InitStates();}
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  std::vector<int> states = {};
  void InitStates() {
    for (int i=0; i<10; i++) {
    	states.push_back(0);
    }
  };
};

#endif