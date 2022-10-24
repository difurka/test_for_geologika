#ifndef COMMANDS_EXECUTION_H_
#define COMMANDS_EXECUTION_H_

#include <mutex>
#include <thread>
#include <queue>
#include <chrono>
#include <iostream>  // @note del
#include <random>

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

  CommandsExecution() {
    std::thread inspector = std::thread(&CommandsExecution::InspectionOfQueue, this);
    inspector.detach();
  };

  ~CommandsExecution() {
    is_run_ = false;
  };
  
  void SetPeriod(double period) {
    is_run_ = false;
    period_ = period;
    is_run_ = true;
  }
   
  double GetPeriod() {return period_;};
  std::queue<command_t> GetCommands() {return commands_;}
  double GetVelocityOfPump() {return velocity_of_pump_;}
  double GetPressureOfSensor1() {return pressure_of_sensor1_;}
  double GetPressureOfSensor2() {return pressure_of_sensor2_;}

  void Push(PartType element, double value){
    std::lock_guard lock(mtx_);
    commands_.push(std::make_pair(element, value));
    has_command_for_execute_ = true;
  }

  void InspectionOfQueue() {
    while (is_run_) {
      if (has_command_for_execute_) {
        ExecuteFirstCommandFromQueue();
        {
          std::lock_guard lock(mtx_);
          if (commands_.empty()) has_command_for_execute_ = false;
        }
        SleepForPeriod();
      } 
      // else {
      //   SetVeluesWhenQueueIsEmpty();
      // }
    }
  } 
  
  void ExecuteFirstCommandFromQueue() {
    command_t command;
    command = GetElementFromQueue();
    PartType part_of_device = command.first;
    double value = command.second;
    if (part_of_device == kPump) {
      pump_.SetVelocity(value);
      velocity_of_pump_ = value;
      std::cout << "Set velocity of the pump: " << value << std::endl;
    }
    if (part_of_device == kSensor1) {
      sensor1_.SetPressure(value);
      pressure_of_sensor1_ = value;
      std::cout << "Set pressure of the sensor1: " << value << std::endl;
    }
    if (part_of_device == kSensor2) {
      sensor2_.SetPressure(value);
      pressure_of_sensor2_ = value;
      std::cout << "Set pressure of the sensor2: " << value << std::endl;
    }
  }

  command_t GetElementFromQueue() {
    std::lock_guard lock(mtx_);
    command_t result = commands_.front();
    commands_.pop();
    if (commands_.empty()) has_command_for_execute_ = false;
    return result;
  }
  
  void SleepForPeriod() {
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    while (end - start < std::chrono::seconds(period_) && is_run_) {
      end = std::chrono::high_resolution_clock::now();
    }
  }

  // void SetVeluesWhenQueueIsEmpty() {
  //   std::random_device rd;
  //   std::mt19937 gen(rd());
  //   std::uniform_real_distribution<> dist(-0.01, 0.01);
  //   std::lock_guard lock(mtx_);
  //   velocity_of_pump_ = (dist(gen) + 1) * pump_.GetVelocity();
  //   pressure_of_sensor1_ = (dist(gen)+ 1) * sensor1_.GetPressure();  //@note if null plus noise
  //   pressure_of_sensor2_ = (dist(gen) + 1) * sensor2_.GetPressure();  //@note if null plus noise
  // }

 private: 
  std::queue<command_t> commands_;
  int period_ = 5;
  Pump pump_;
  Sensor sensor1_, sensor2_;
  const double kPercentOfNoise = 1;
  mutable std::mutex mtx_;
  bool has_command_for_execute_ = false;
  bool is_run_ = true;
  // double velocity_of_pump_ = 0;   // @note move to Pump
  // double pressure_of_sensor1_ = 0;  // @note move to Sensor
  // double pressure_of_sensor2_ = 0;  // @note move to Sensor
};


#endif  // COMMANDS_EXECUTION_
