#pragma once

#include <stdint.h>

struct ServoHardwareConfig {
  uint8_t pin;
  uint16_t frequencyHz;
  int minPulseWidthUs;
  int maxPulseWidthUs;
};

struct SweepConfig {
  int startAngle;
  int endAngle;
  uint32_t stepIntervalMs;
  uint32_t endpointPauseMs;
};
