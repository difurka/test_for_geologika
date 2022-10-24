#ifndef QUEUE_OF_COMMANDS_H_
#define QUEUE_OF_COMMANDS_H_

#include <mutex>
#include <thread>
#include <queue>
#include <chrono>
#include <iostream>  // @note del
#include <random>

#include "elements_of_device/pump.h"
#include "elements_of_device/sensor.h"

class QueueOfCommands {
 public:
  enum PartType {
    kPump,
    kSensor1,
    kSensor2
  };
 public:
  QueueOfCommands() {
    std::thread inspector = std::thread(&QueueOfCommands::InspectionOfQueue, this);
    inspector.detach();
  };

  ~QueueOfCommands() {
    is_run_ = false;
    // std::cout << "End" <<  std::endl;
  };
  
  void SetPeriod(double period) {
    is_run_ = false;
    period_ = period;
    is_run_ = true;
  }
   
  double GetPeriod() {return period_;};
  std::queue<std::pair<PartType, double>> GetCommands() {return commands_;}
  double GetVelocityOfPump() {return velocity_of_pump_;}
  double GetPressureOfSensor1() {return pressure_of_sensor1_;}
  double GetPressureOfSensor2() {return pressure_of_sensor2_;}

  void Push(PartType element, double value){
    std::lock_guard lock(mtx_);
    commands_.push(std::make_pair(element, value));
    update_ = true;
  }

  void InspectionOfQueue() {
    while (is_run_) {
      if (update_) {
        ExecuteFirstCommandFromQueue();
        {
          std::lock_guard lock(mtx_);
          if (commands_.empty()) update_ = false;
        }
        SleepForPeriod();
      } else {
        SetVeluesWhenQueueIsEmpty();
      }
    }
  } 
  
  void ExecuteFirstCommandFromQueue() {
    std::pair<PartType, double> command;
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

  std::pair<PartType, double> GetElementFromQueue() {
    std::lock_guard lock(mtx_);
    std::pair<PartType, double>  result = commands_.front();
    commands_.pop();
    if (commands_.empty()) update_ = false;
    return result;
  }
  
  void SleepForPeriod() {
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    while (end - start < std::chrono::seconds(period_) && is_run_) {
      end = std::chrono::high_resolution_clock::now();
    }
  }

  void SetVeluesWhenQueueIsEmpty() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.01, 0.01);
    // velocity_of_pump_ = (dist(gen) + 1) * pump_.GetVelocity();
    // pressure_of_sensor1_ = (dist(gen)+ 1) * sensor1_.GetPressure();  //@note if null plus noise
    // pressure_of_sensor2_ = (dist(gen) + 1) * sensor2_.GetPressure();  //@note if null plus noise
  }

 private: 
  std::queue<std::pair<PartType, double>> commands_;
  int period_ = 5;
  mutable std::mutex mtx_;
  bool update_ = false;
  bool is_run_ = true;
  Pump pump_;
  Sensor sensor1_, sensor2_;
  double velocity_of_pump_ = 0;   // @note move to Pump
  double pressure_of_sensor1_ = 0;  // @note move to Sensor
  double pressure_of_sensor2_ = 0;  // @note move to Sensor
};


#endif  // SQUEUE_OF_COMMANDS_H_
