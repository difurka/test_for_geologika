#ifndef COMMANDS_EXECUTION_H_
#define COMMANDS_EXECUTION_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "elements_of_device/pump.h"
#include "elements_of_device/sensor.h"

class CommandsExecution {
 public:
  enum PartType { kPump, kSensor1, kSensor2 };
  using command_t = std::pair<PartType, double>;

  CommandsExecution();
  ~CommandsExecution();

  void SetPeriod(double period);

  void PushInQueue(PartType element, double value);
  void InspectionOfQueue();
  void ExecuteFirstCommandFromQueue();
  command_t GetElementFromQueue();
  void SleepForPeriod();
  void SetVeluesWhenQueueIsEmpty();
  void GetInfo();

 private:
  std::queue<command_t> commands_;
  int period_ = 5;
  Pump pump_;
  Sensor sensor1_, sensor2_;
  const double kPercentOfNoise = 1.0;

  mutable std::mutex mtx_;
  std::atomic<bool> end_of_cycle_in_thread_ = false;
  std::condition_variable ready_to_close_thread_;
  bool need_thread_for_run_ = true;
  bool has_command_for_execute_ = false;
  double difference_of_pressures_ = 0;
};

#endif  // COMMANDS_EXECUTION_
