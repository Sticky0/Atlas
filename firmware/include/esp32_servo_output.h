#pragma once

#include <ESP32Servo.h>

#include "angle_output.h"
#include "servo_config.h"

class Esp32ServoOutput final : public IAngleOutput {
 public:
  explicit Esp32ServoOutput(const ServoHardwareConfig& config);

  bool begin();
  void setAngle(int angle) override;

 private:
  const ServoHardwareConfig& config_;
  Servo servo_;
  bool attached_ = false;
};
