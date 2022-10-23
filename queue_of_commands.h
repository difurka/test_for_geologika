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
    is_run_ = false;  // @note wake thread
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
    std::pair<PartType, double> command;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1, 1);

    while (is_run_) {
      if (update_) {
        command = GetElementFromQueue();
        PartType part = command.first;
        double value = command.second;
        if (part == kPump) {
          pump_.SetVelocity(value);
          velocity_of_pump_ = value;
          std::cout << "Set velocity of the pump: " << value << std::endl;
        }
        if (part == kSensor1) {
          sensor1_.SetPressure(value);
          pressure_of_sensor1_ = value;
          std::cout << "Set pressure of the sensor1: " << value << std::endl;
        }
        if (part == kSensor2) {
          sensor2_.SetPressure(value);
          pressure_of_sensor2_ = value;
          std::cout << "Set pressure of the sensor2: " << value << std::endl;
        }
        if (commands_.empty()) update_ = false;
        std::this_thread::sleep_for(std::chrono::seconds(period_));
      } else {
         velocity_of_pump_ = (dist(gen)/100 + 1)  * pump_.GetVelocity();
         pressure_of_sensor1_ = (dist(gen)/100 + 1) * sensor1_.GetPressure();  //@note if null plus noise
         pressure_of_sensor2_ = (dist(gen)/100 + 1)  * sensor2_.GetPressure();  //@note if null plus noise
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


 private:
  bool update_ = false;
  bool is_run_ = true;
  std::queue<std::pair<PartType, double>> commands_;
  int period_ = 10;
  mutable std::mutex mtx_;
  Pump pump_;
  Sensor sensor1_, sensor2_;
  double velocity_of_pump_ = 0;
  double pressure_of_sensor1_ = 0;
  double pressure_of_sensor2_ = 0;
};


#endif  // SQUEUE_OF_COMMANDS_H_
