#include "esp32_servo_output.h"

Esp32ServoOutput::Esp32ServoOutput(const ServoHardwareConfig& config)
    : config_(config) {}

bool Esp32ServoOutput::begin() {
  if (config_.frequencyHz == 0 ||
      config_.minPulseWidthUs >= config_.maxPulseWidthUs) {
    return false;
  }

  servo_.setPeriodHertz(config_.frequencyHz);
  servo_.attach(
      config_.pin,
      config_.minPulseWidthUs,
      config_.maxPulseWidthUs);
  attached_ = servo_.attached();
  return attached_;
}

void Esp32ServoOutput::setAngle(const int angle) {
  if (attached_) {
    servo_.write(angle);
  }
}
