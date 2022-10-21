#include "implementation.h"

#include <iostream>
#include <regex>

void Implementation::Start() {
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

void Implementation::ReadCommandFromConsole(const std::string& command) {
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

void Implementation::Print(const Message &message) {
  std::cout << messages_[message];
  std::cout << std::endl;
}

void Implementation::PrintInfo() {
  std::cout << "Velocity of pump:" <<  pump.GetVelocity() << std::endl;
  std::cout << "Pressure of sensor 1: " <<  sensor1.GetPressure() << std::endl;
  std::cout << "Pressure of sensor 2: " <<  sensor2.GetPressure() << std::endl;
  std::cout << "Period: " <<  period_ << std::endl;

  std::queue<std::pair<PartType, double>> tempQueue = queueOfCommands;
  std::cout << "The queue of commands: \n";
  if (tempQueue.empty())
    std::cout << "empty." << std::endl;
  else {
    int i = 1;
    while (!tempQueue.empty()) {
      std::cout << i << ") " << parts_[tempQueue.front().first] << " " << tempQueue.front().second << "\n";
      tempQueue.pop();
      i++;
    }
  }
  
}

void Implementation::CommandForDevice(const std::string& command) {
  if (std::regex_search(command, std::regex(regex_[kSetVelocityOfPamp]))) {
    AddVelocityOfPamp(command);
  } else if (std::regex_search(command, std::regex(regex_[kSetPressureOfSensor1]))) {
    AddPressureOfSensor1(command);
  } else if (std::regex_search(command, std::regex(regex_[kSetPressureOfSensor2]))) {
    AddPressureOfSensor2(command);
  } else if (std::regex_search(command, std::regex(regex_[kSetPeriod]))) {
    ChagePeriodOfDevice(command);
  } else {
    std::cout << "ERROR: invalid command\n" << command << std::endl;  // @note del command
  }
}

void Implementation::AddVelocityOfPamp(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double velocity = std::stod(tokens[1]);
  queueOfCommands.push(std::make_pair(kPump, velocity));
  Print(kSuccessOfAddVelocityOfPamp);
}

void Implementation::AddPressureOfSensor1(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double pressure= std::stod(tokens[1]);
  queueOfCommands.push(std::make_pair(kSensor1, pressure));
  Print(kSuccessOfAddPressureOfSensor1);
}

void Implementation::AddPressureOfSensor2(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double pressure = std::stod(tokens[1]);
  queueOfCommands.push(std::make_pair(kSensor2, pressure));
  Print(kSuccessOfAddPressureOfSensor2);
}

void Implementation::ChagePeriodOfDevice(const std::string& command) {

  auto tokens = ParserOfCommand(command);
  double period = std::stod(tokens[1]);
  period_ = period;
  Print(kSuccessOfSetPeriod);
}

std::vector<std::string> Implementation::ParserOfCommand(const std::string& command) {
  std::vector<std::string> result;
  std::istringstream string_stream(RemoveSpaces(command));
  std::string token;
  while (std::getline(string_stream, token, ' ')) {
    result.push_back(token);
  }
  return result;
}

std::string Implementation::RemoveSpaces(const std::string& command) {
  std::regex sample("[ ]{1,}");
  std::string result;
  std::regex_replace(std::back_inserter(result),
                    command.begin(), command.end(), sample, " ");
  return result;
}