#include "servo_sweep.h"

ServoSweep::ServoSweep(IAngleOutput& output,
                       const IClock& clock,
                       const SweepConfig& config)
    : output_(output),
      clock_(clock),
      config_(config),
      currentAngle_(config.startAngle),
      direction_(config.startAngle <= config.endAngle ? 1 : -1) {}

bool ServoSweep::begin() {
  if (!isValidConfig()) {
    return false;
  }

  currentAngle_ = config_.startAngle;
  direction_ = config_.startAngle <= config_.endAngle ? 1 : -1;
  output_.setAngle(currentAngle_);
  scheduleNextStep(clock_.nowMs(), 0);
  running_ = true;
  return true;
}

void ServoSweep::update() {
  if (!running_) {
    return;
  }

  const uint32_t now = clock_.nowMs();
  if (!hasReachedNextStep(now)) {
    return;
  }

  currentAngle_ += direction_;
  output_.setAngle(currentAngle_);

  if (currentAngle_ == config_.endAngle ||
      currentAngle_ == config_.startAngle) {
    direction_ = -direction_;
    scheduleNextStep(now, config_.endpointPauseMs);
    return;
  }

  scheduleNextStep(now, config_.stepIntervalMs);
}

bool ServoSweep::isValidConfig() const {
  const bool validAngles = config_.startAngle >= 0 &&
                           config_.startAngle <= 180 &&
                           config_.endAngle >= 0 &&
                           config_.endAngle <= 180 &&
                           config_.startAngle != config_.endAngle;
  return validAngles && config_.stepIntervalMs > 0;
}

bool ServoSweep::hasReachedNextStep(const uint32_t now) const {
  return static_cast<int32_t>(now - nextStepAtMs_) >= 0;
}

void ServoSweep::scheduleNextStep(const uint32_t now,
                                  const uint32_t delayMs) {
  nextStepAtMs_ = now + delayMs;
}
