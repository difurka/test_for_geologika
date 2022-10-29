#include "read_from_consol.h"

#include <iostream>
#include <regex>

void ReadFromConsol::StartReadFromConsol() {
  std::string line;
  Print(kWelcome);
  while (line != "EXIT" && line != "exit" && !std::cin.eof()) {
    try {
      std::getline(std::cin, line);
      ReadCommandFromConsole(line);
    } catch (std::exception& ex) {
      std::cout << "ERROR: " << ex.what() << std::endl;
    }
  }
}

void ReadFromConsol::ReadCommandFromConsole(const std::string& command) {
  if ((command == "HELP") || (command == "help")) {
    Print(kShowHelp);
    Print(kWelcome);
  } else if ((command == "INFO") || (command == "info")) {
    commands_execution_.GetInfo();
    Print(kWelcome);
  } else if (command == "EXIT" || command == "exit") {
    std::cout << "End of device operation" << std::endl;
  } else {
    CommandForDevice(command);
  }
}

void ReadFromConsol::Print(const Message& message) {
  std::cout << messages_[message];
  std::cout << std::endl;
}

void ReadFromConsol::CommandForDevice(const std::string& command) {
  auto tokens = ParserOfCommand(command);

  if (tokens.size() != 2)
    Print(kErrorInCommand);
  else {
    try {
      std::string part_of_device = tokens[0];
      double value = stod(tokens[1]);
      if (part_of_device == "t" || part_of_device =="T") {
        commands_execution_.SetPeriod(value);
        Print(kSuccessOfSetPeriod);
      } else if (part_of_device == "p" || part_of_device =="P") {
        commands_execution_.PushInQueue(CommandsExecution::kPump, value);
        Print(kSuccessOfAddVelocityOfPamp);
      } else if (part_of_device == "s1" || part_of_device =="S1") {
        commands_execution_.PushInQueue(CommandsExecution::kSensor1, value);
        Print(kSuccessOfAddPressureOfSensor1);
      } else if (part_of_device == "s2" || part_of_device =="S2") {
        commands_execution_.PushInQueue(CommandsExecution::kSensor2, value);
        Print(kSuccessOfAddPressureOfSensor2);
      } else {
        Print(kErrorInCommand);
      }
    } catch (std::exception& ex) {
      Print(kErrorInCommand);
    }
  }
}

std::vector<std::string> ReadFromConsol::ParserOfCommand(
    const std::string& command) {
  std::vector<std::string> result;
  std::istringstream string_stream(command);
  std::string token;
  while (std::getline(string_stream, token, ' ')) {
    result.push_back(token);
  }
  return result;
}
