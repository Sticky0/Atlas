#pragma once

#include "angle_output.h"
#include "clock.h"
#include "servo_config.h"

class ServoSweep final {
 public:
  ServoSweep(IAngleOutput& output,
             const IClock& clock,
             const SweepConfig& config);

  bool begin();
  void update();

 private:
  bool isValidConfig() const;
  bool hasReachedNextStep(uint32_t now) const;
  void scheduleNextStep(uint32_t now, uint32_t delayMs);

  IAngleOutput& output_;
  const IClock& clock_;
  const SweepConfig& config_;
  int currentAngle_;
  int direction_;
  uint32_t nextStepAtMs_ = 0;
  bool running_ = false;
};
