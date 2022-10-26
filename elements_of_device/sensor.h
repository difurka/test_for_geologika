#ifndef ELEMENT_OF_DEVICE_SENSOR_H_
#define ELEMENT_OF_DEVICE_SENSOR_H_

#include <random>

class Sensor {
 public:
  void SetPressure(double pressure) {
    pressure_ = pressure;
    pressure_with_noise_ = pressure;
  }

  double GetPressure() {
    return pressure_with_noise_;
  }

  void AddNoiseToPressure(double percent) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.01, 0.01);
    pressure_with_noise_ = (dist(gen) * percent + 1) * pressure_;
  }

 private:
  double pressure_ = 0;
  double pressure_with_noise_ = 0;
};
#endif  // EELEMENT_OF_DEVICE_SENSORH_
