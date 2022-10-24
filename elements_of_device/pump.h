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

 private:
  double velocity_ = 0;
  // double velocity_without_command_ = 0;
};
#endif  // EELEMENT_OF_DEVICE_PUMP_H_
