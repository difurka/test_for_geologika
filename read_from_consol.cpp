#include "read_from_consol.h"

#include <iostream>
#include <iomanip>
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
    PrintInfo();
    Print(kWelcome);
  } else if (command == "EXIT" || command == "exit") {
    std::cout << "End of device operation" << std::endl;
  } else {
    CommandForDevice(command);
  }
}

void ReadFromConsol::Print(const Message &message) {
  std::cout << messages_[message];
  std::cout << std::endl;
}

void ReadFromConsol::PrintInfo() {
  std::vector<std::string> parts_ = {"Pump", "Sensor1", "Sensor2"};
  std::cout << std::fixed << std::setprecision(3) 
            << "Velocity of pump: " <<  commands_execution_.GetVelocityOfPump() << std::endl;
  std::cout << std::fixed << std::setprecision(3)
            << "Pressure of sensor 1: " <<  commands_execution_.GetPressureOfSensor1() << std::endl;
  std::cout << std::fixed << std::setprecision(3)
            << "Pressure of sensor 2: " <<  commands_execution_.GetPressureOfSensor2() << std::endl;
  std::cout << "Period: " <<  commands_execution_.GetPeriod() << std::endl;

  std::queue<CommandsExecution::command_t> tempQueue = commands_execution_.GetCommands();
  std::cout << "The queue of commands: ";
  if (tempQueue.empty()) {
    std::cout << "empty." << std::endl;
    std::cout << std::fixed << std::setprecision(3)
              << "Pressure difference between sensor 1 and sensor 2: " 
              << commands_execution_.GetPressureOfSensor1() - commands_execution_.GetPressureOfSensor2()
              << std::endl;
  } else {
    int i = 1;
    while (!tempQueue.empty()) {
      std::cout << "\n" << i << ") " << parts_[tempQueue.front().first] 
      << " " << tempQueue.front().second;
      tempQueue.pop();
      i++;
    }
    std::cout << "\nend of queue.\n" << std::endl;
  }
}

void ReadFromConsol::CommandForDevice(const std::string& command) {
  if (std::regex_search(command, std::regex(regex_[kSetVelocityOfPamp]))) {
    AddVelocityOfPamp(command);
  } else if (std::regex_search(command, std::regex(regex_[kSetPressureOfSensor1]))) {
    AddPressureOfSensor1(command);
  } else if (std::regex_search(command, std::regex(regex_[kSetPressureOfSensor2]))) {
    AddPressureOfSensor2(command);
  } else if (std::regex_search(command, std::regex(regex_[kSetPeriod]))) {
    ChagePeriodOfDevice(command);
  } else {
    std::cout << "ERROR: invalid command " << command << std::endl;
  }
}

void ReadFromConsol::AddVelocityOfPamp(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double velocity = std::stod(tokens[1]);
  commands_execution_.PushInQueue(CommandsExecution::kPump, velocity);
  Print(kSuccessOfAddVelocityOfPamp);
}

void ReadFromConsol::AddPressureOfSensor1(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double pressure = std::stod(tokens[1]);
  commands_execution_.PushInQueue(CommandsExecution::kSensor1, pressure);
  Print(kSuccessOfAddPressureOfSensor1);
}

void ReadFromConsol::AddPressureOfSensor2(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double pressure = std::stod(tokens[1]);
  commands_execution_.PushInQueue(CommandsExecution::kSensor2, pressure);
  Print(kSuccessOfAddPressureOfSensor2);
}

void ReadFromConsol::ChagePeriodOfDevice(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double period = std::stod(tokens[1]);
  commands_execution_.SetPeriod(period);
  Print(kSuccessOfSetPeriod);
}

std::vector<std::string> ReadFromConsol::ParserOfCommand(const std::string& command) {
  std::vector<std::string> result;
  std::istringstream string_stream(command);
  std::string token;
  while (std::getline(string_stream, token, ' ') ) {
    result.push_back(token);
  }
  return result;
}
