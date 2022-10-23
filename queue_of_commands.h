#ifndef QUEUE_OF_COMMANDS_H_
#define QUEUE_OF_COMMANDS_H_

#include <mutex>
#include <queue>

class QueueOfCommands {
 public:
  enum PartType {
    kPump,
    kSensor1,
    kSensor2
  };
 public:
  QueueOfCommands() = default;
  ~QueueOfCommands() = default;
  
  void SetPeriod(double period) {period_ = period;}
  double GetPeriod() {return period_;};
  std::queue<std::pair<PartType, double>> GetCommands() {return commands_;}

  void Push(PartType element, double value){
    commands_.push(std::make_pair(element, value));
  }

 private:
  // bool update_ = false;
  // bool is_run_ = true;
  std::queue<std::pair<PartType, double>> commands_;
  double period_ = 5;
  mutable std::mutex mtx_;

};


#endif  // SQUEUE_OF_COMMANDS_H_
