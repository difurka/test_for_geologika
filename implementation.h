#ifndef IMPLEMENTATION_H_
#define IMPLEMENTATION_H_

#include <string>
// #include <queue>
// #include <map>
// #include <stdexcept>

// #include "elements_of_device/pump.h"
// #include "elements_of_device/sensor.h"
#include "queue_of_commands.h"

class Implementation {
  // enum PartType {
  //   kPump,
  //   kSensor1,
  //   kSensor2
  // };

  enum Message {
    kWelcome,
    kShowHelp,
    kSuccessOfAddVelocityOfPamp,
    kSuccessOfAddPressureOfSensor1,
    kSuccessOfAddPressureOfSensor2,
    kSuccessOfSetPeriod
  };

  enum Command {
    kSetVelocityOfPamp,
    kSetPressureOfSensor1,
    kSetPressureOfSensor2,
    kSetPeriod, 
    kGetInfo
  };   
 public:
  Implementation() = default;
  ~Implementation() = default;
  void Start();
  void ReadCommandFromConsole(const std::string& command);
  void CommandForDevice(const std::string& command);
  void Print(const Message &message);
  void PrintInfo();
  std::string RemoveSpaces(const std::string& command);
  std::vector<std::string> ParserOfCommand(const std::string& command);
  void AddVelocityOfPamp(const std::string& command);
  void AddPressureOfSensor1(const std::string& command);
  void AddPressureOfSensor2(const std::string& command);
  void ChagePeriodOfDevice(const std::string& command);
  

 private:
  QueueOfCommands queueOfCommands_;
  // Pump pump_;
  // Sensor sensor1_, sensor2_;
  // double period_ = 5;
  const std::vector<std::string> messages_ {
    "\n(EXIT)            enter for exit the program\n"\
     "(HELP)            get help\n",
   
    "\n(P V)             add velocity V for pamp in the queue,\n"\
      "(S1 P)            add pressure P for sensor 1 in the queue,\n"\
      "(S2 P)            set pressure P for sensor 2 in the queue,\n"\
      "(T S)             set period S of operation for the device in sec, the queue will set empty.\n"\
      "(INFO)            get info about current settings of the device, content of queue of commands.",

    "Velocity of the pump has added to queue.\n",

    "Pressure of the sensor 1 has added to queue.\n",

    "Pressure of the sensor 2 has added to queue.\n",

    "Period has chanded.\n"
  };

  const std::vector<std::string> regex_ {
    "^(P|p)[ ]{1,}+\\d*\\.?\\d+",
    "^(S1|s1)[ ]{1,}+\\d*\\.?\\d+",
    "^(S2|s2)[ ]{1,}+\\d*\\.?\\d+",
    "^(T|t)[ ]{1,}+\\d*\\.?\\d+"
  };
  const std::vector<std::string> parts_ = {"Pump", "Sensor1", "Sensor2"};
};
#endif  // IMPLEMENTATION__H_
