#ifndef ELEMENT_OF_DEVICE_SENSOR_H_
#define ELEMENT_OF_DEVICE_SENSOR_H_

class Sensor {
 public:
  void SetPressure(double pressure) {
    pressure_ = pressure;
  }

  double GetPressure() {
    return pressure_;
  }

  double GetPressureWithNoise(double percent) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.01, 0.01);
    return (dist(gen) * percent + 1) * pressure_;
  }

 private:
  double pressure_ = 0;
};
#endif  // EELEMENT_OF_DEVICE_SENSORH_
