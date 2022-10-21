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

 private:
  double pressure_ = 0;
};
#endif  // EELEMENT_OF_DEVICE_SENSORH_
