#ifndef ELEMENT_OF_DEVICE_PUMP_H_
#define ELEMENT_OF_DEVICE_PUMP_H_

class Pump {
 public:
  void SetVelocity(double velocity) {
    velocity_ = velocity;
  }

  double GetVelocity() {
    return velocity_;
  }

  double GetVelocityWithNoise(double percent) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.01, 0.01);
    return (dist(gen) * percent + 1) * velocity_;
  }
   

 private:
  double velocity_ = 0;
  // double velocity_without_command_ = 0;
};
#endif  // EELEMENT_OF_DEVICE_PUMP_H_
