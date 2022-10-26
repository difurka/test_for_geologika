#include "commands_execution.h"

#include <chrono>
#include <iostream>

CommandsExecution::CommandsExecution() {
  std::thread inspector =
      std::thread(&CommandsExecution::InspectionOfQueue, this);
  inspector.detach();
};

CommandsExecution::~CommandsExecution() {
  need_thread_for_run_ = false;
  std::unique_lock<std::mutex> lck(mtx_);
  ready_to_close_thread_.wait(lck,
                              [&] { return end_of_cycle_in_thread_.load(); });
};

void CommandsExecution::SetPeriod(double period) {
  need_thread_for_run_ = false;
  period_ = period;
  need_thread_for_run_ = true;
}

double CommandsExecution::GetPeriod() { return period_; };
std::queue<CommandsExecution::command_t> CommandsExecution::GetCommands() {
  return commands_;
}
double CommandsExecution::GetVelocityOfPump() { return pump_.GetVelocity(); }
double CommandsExecution::GetPressureOfSensor1() {
  return sensor1_.GetPressure();
}
double CommandsExecution::GetPressureOfSensor2() {
  return sensor2_.GetPressure();
}

void CommandsExecution::PushInQueue(PartType element, double value) {
  std::lock_guard lock(mtx_);
  commands_.push(std::make_pair(element, value));
  has_command_for_execute_ = true;
}

void CommandsExecution::InspectionOfQueue() {
  while (need_thread_for_run_) {
    end_of_cycle_in_thread_ = false;
    if (has_command_for_execute_) {
      ExecuteFirstCommandFromQueue();
      {
        std::lock_guard lock(mtx_);
        if (commands_.empty()) has_command_for_execute_ = false;
      }
      SleepForPeriod();
    } else {
      SetVeluesWhenQueueIsEmpty();
    }
    end_of_cycle_in_thread_ = true;
  }
  ready_to_close_thread_.notify_one();
}

void CommandsExecution::ExecuteFirstCommandFromQueue() {
  command_t command;
  command = GetElementFromQueue();
  PartType part_of_device = command.first;
  double value = command.second;
  if (part_of_device == kPump) {
    pump_.SetVelocity(value);
    std::cout << "Set velocity of the pump: " << value << std::endl;
  }
  if (part_of_device == kSensor1) {
    sensor1_.SetPressure(value);
    std::cout << "Set pressure of the sensor1: " << value << std::endl;
  }
  if (part_of_device == kSensor2) {
    sensor2_.SetPressure(value);
    std::cout << "Set pressure of the sensor2: " << value << std::endl;
  }
}

CommandsExecution::command_t CommandsExecution::GetElementFromQueue() {
  std::lock_guard lock(mtx_);
  command_t result = commands_.front();
  commands_.pop();
  if (commands_.empty()) has_command_for_execute_ = false;
  return result;
}

void CommandsExecution::SleepForPeriod() {
  auto start = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  while (end - start < std::chrono::seconds(period_) && need_thread_for_run_) {
    end = std::chrono::high_resolution_clock::now();
  }
}

void CommandsExecution::SetVeluesWhenQueueIsEmpty() {
  pump_.AddNoiseToVelocity(kPercentOfNoise);
  sensor1_.AddNoiseToPressure(kPercentOfNoise);
  sensor2_.AddNoiseToPressure(kPercentOfNoise);
}