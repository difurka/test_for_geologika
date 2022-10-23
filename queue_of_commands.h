#ifndef QUEUE_OF_COMMANDS_H_
#define QUEUE_OF_COMMANDS_H_

#include <mutex>
#include <thread>
#include <queue>
#include <chrono>
#include <iostream>  // @note del

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
  };
  
  void SetPeriod(double period) {period_ = period;}
  double GetPeriod() {return period_;};
  std::queue<std::pair<PartType, double>> GetCommands() {return commands_;}

  void Push(PartType element, double value){
    std::lock_guard lock(mtx_);
    commands_.push(std::make_pair(element, value));
    update_ = true;
  }

  void InspectionOfQueue() {
    // std::cout << "in thread" << std::endl;
    
    std::pair<PartType, double> command;
    while (is_run_) {
      if (update_) {
        command = GetElementFromQueue();

        if (command.first == kPump) {
          pump_.SetVelocity(command.second);
          std::cout << "Set velocity of the pump: " << command.second << std::endl;
        }
        if (command.first == kSensor1) {
          sensor1_.SetPressure(command.second);
          std::cout << "Set pressure of the sensor1: " << command.second << std::endl;
        }
        if (command.first == kSensor2) {
          sensor2_.SetPressure(command.second);
          std::cout << "Set pressure of the sensor2: " << command.second << std::endl;
        }
        if (commands_.empty()) update_ = false;
        std::this_thread::sleep_for(std::chrono::seconds(period_));
      }
    }
  } 
  
    std::pair<PartType, double> GetElementFromQueue() {
      std::lock_guard lock(mtx_);
      std::pair<PartType, double>  result = commands_.front();
      commands_.pop();
      if (commands_.empty()) update_ = false;
      return result;
    }

  Pump pump_;
  Sensor sensor1_, sensor2_;
 private:
  bool update_ = false;
  bool is_run_ = true;
  std::queue<std::pair<PartType, double>> commands_;
  int period_ = 10;
  mutable std::mutex mtx_;
};


#endif  // SQUEUE_OF_COMMANDS_H_
