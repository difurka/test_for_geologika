#ifndef READ_FROM_CONSOL_H_
#define READ_FROM_CONSOL__H_

#include <string>

#include "commands_execution.h"

class ReadFromConsol {
  enum Message {
    kWelcome,
    kShowHelp,
    kSuccessOfAddVelocityOfPamp,
    kSuccessOfAddPressureOfSensor1,
    kSuccessOfAddPressureOfSensor2,
    kSuccessOfSetPeriod,
    kErrorInCommand
  };

  enum Command {
    kSetVelocityOfPamp,
    kSetPressureOfSensor1,
    kSetPressureOfSensor2,
    kSetPeriod,
    kGetInfo
  };

 public:
  ReadFromConsol() = default;
  ~ReadFromConsol() = default;

  void StartReadFromConsol();
  void ReadCommandFromConsole(const std::string& command);
  std::vector<std::string> ParserOfCommand(const std::string& command);
  void CommandForDevice(const std::string& command);

  void Print(const Message& message);

 private:
  CommandsExecution commands_execution_;

  const std::vector<std::string> messages_{
      "\n(EXIT)            enter for exit the program\n"
      "(HELP)            get help\n",

      "\n(P V)             add velocity V for pamp in the queue,\n"
      "(S1 P)            add pressure P for sensor 1 in the queue,\n"
      "(S2 P)            set pressure P for sensor 2 in the queue,\n"
      "(T S)             set period S of operation for the device in sec, the "
      "queue will set empty.\n"
      "(INFO)            get info about current settings of the device, "
      "content of queue of commands.\n",

      "Velocity of the pump has added to queue.\n",

      "Pressure of the sensor 1 has added to queue.\n",

      "Pressure of the sensor 2 has added to queue.\n",

      "Period has chanded.\n",
      
      "ERROR: invalid command \n"};
};
#endif  // READ_FROM_CONSOL_H_
