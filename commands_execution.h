#ifndef COMMANDS_EXECUTION_H_
#define COMMANDS_EXECUTION_H_

#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>

#include "elements_of_device/pump.h"
#include "elements_of_device/sensor.h"

class CommandsExecution {  
 public:
  enum PartType {
    kPump,
    kSensor1,
    kSensor2
  };
  using command_t = std::pair<PartType, double>; 

  CommandsExecution();
  ~CommandsExecution();
  
  void SetPeriod(double period);
   
  double GetPeriod();
  std::queue<command_t> GetCommands();
  double GetVelocityOfPump();
  double GetPressureOfSensor1();
  double GetPressureOfSensor2();

  void PushInQueue(PartType element, double value);
  void InspectionOfQueue();
  void ExecuteFirstCommandFromQueue();
  command_t GetElementFromQueue();
  void SleepForPeriod();
  void SetVeluesWhenQueueIsEmpty();

 private: 
  std::queue<command_t> commands_;
  int period_ = 5;
  Pump pump_;
  Sensor sensor1_, sensor2_;
  const double kPercentOfNoise = 1.0;

  mutable std::mutex mtx_;
  std::atomic<bool> end_of_cycle_in_thread_ = false;
  std::condition_variable condVar_;
  bool need_thread_for_run_ = true;
  bool has_command_for_execute_ = false;
};


#endif  // COMMANDS_EXECUTION_
