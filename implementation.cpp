#include "implementation.h"

#include <iostream>
#include <iomanip>
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
  std::cout << "Velocity of pump: " <<  queueOfCommands_.GetVelocityOfPump() << std::endl;
  std::cout << "Pressure of sensor 1: " <<  queueOfCommands_.GetPressureOfSensor1() << std::endl;
  std::cout << "Pressure of sensor 2: " <<  queueOfCommands_.GetPressureOfSensor2() << std::endl;
  std::cout << "Period: " <<  queueOfCommands_.GetPeriod() << std::endl;

  std::queue<std::pair<QueueOfCommands::PartType, double>> tempQueue = queueOfCommands_.GetCommands();
  std::cout << "The queue of commands: ";
  if (tempQueue.empty()) {
    std::cout << "empty." << std::endl;
    std::cout << std::fixed << std::setprecision(3)
              << "Pressure difference between sensor 1 and sensor 2: " 
              << queueOfCommands_.GetPressureOfSensor1() - queueOfCommands_.GetPressureOfSensor2()
              << std::endl;
  } else {
    int i = 1;
    while (!tempQueue.empty()) {
      std::cout << "\n" << i << ") " << parts_[tempQueue.front().first] << " " << tempQueue.front().second;
      tempQueue.pop();
      i++;
    }
    std::cout << "\nend of queue.\n" << std::endl;
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
    std::cout << "ERROR: invalid command " << command << std::endl;
  }
}

void Implementation::AddVelocityOfPamp(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double velocity = std::stod(tokens[1]);
  queueOfCommands_.Push(QueueOfCommands::kPump, velocity);
  // Print(kSuccessOfAddVelocityOfPamp);
}

void Implementation::AddPressureOfSensor1(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double pressure = std::stod(tokens[1]);
  queueOfCommands_.Push(QueueOfCommands::kSensor1, pressure);
  // Print(kSuccessOfAddPressureOfSensor1);
}

void Implementation::AddPressureOfSensor2(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double pressure = std::stod(tokens[1]);
  queueOfCommands_.Push(QueueOfCommands::kSensor2, pressure);
  // Print(kSuccessOfAddPressureOfSensor2);
}

void Implementation::ChagePeriodOfDevice(const std::string& command) {
  auto tokens = ParserOfCommand(command);
  double period = std::stod(tokens[1]);
  queueOfCommands_.SetPeriod(period);
  // Print(kSuccessOfSetPeriod);
}

std::vector<std::string> Implementation::ParserOfCommand(const std::string& command) {
  std::vector<std::string> result;
  // std::istringstream string_stream(RemoveSpaces(command));
  std::istringstream string_stream(command);
  std::string token;
  while (std::getline(string_stream, token, ' ') && result.size() < 2) {
    result.push_back(token);
  }
  return result;
}

// std::string Implementation::RemoveSpaces(const std::string& command) {
//   std::regex sample("[ ]{1,}");
//   std::string result;
//   std::regex_replace(std::back_inserter(result),
//                     command.begin(), command.end(), sample, " ");
//   return result;
// }