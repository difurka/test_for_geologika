#include "commands_execution.h"

#include <chrono>
#include <iomanip>
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
    } else {
      SetVeluesWhenQueueIsEmpty();
    }
    difference_of_pressures_ = sensor1_.GetPressure() - sensor2_.GetPressure();
    SleepForPeriod();
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

void CommandsExecution::GetInfo() {
  std::vector<std::string> parts_ = {"Pump", "Sensor1", "Sensor2"};
  std::lock_guard lock(mtx_);
  std::cout << std::fixed << std::setprecision(3)
            << "Velocity of pump: " << pump_.GetVelocity()
            << std::endl;
  std::cout << std::fixed << std::setprecision(4) << "Pressure of sensor 1: "
            << sensor1_.GetPressure() << std::endl;
  std::cout << std::fixed << std::setprecision(4) << "Pressure of sensor 2: "
            << sensor2_.GetPressure() << std::endl;
  std::cout << "Period: " << period_ << std::endl;

  std::queue<CommandsExecution::command_t> tempQueue = commands_;
  std::cout << "The queue of commands: ";
  if (tempQueue.empty()) {
    std::cout << "empty." << std::endl;
    std::cout << std::fixed << std::setprecision(4)
              << "Pressure difference between sensor 1 and sensor 2: "
              << difference_of_pressures_
              << std::endl;
  } else {
    int i = 1;
    while (!tempQueue.empty()) {
      std::cout << "\n"
                << i << ") " << parts_[tempQueue.front().first] << " "
                << tempQueue.front().second;
      tempQueue.pop();
      i++;
    }
    std::cout << "\nend of queue.\n" << std::endl;
  }
}