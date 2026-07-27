#include "arduino_clock.h"

#include <Arduino.h>

uint32_t ArduinoClock::nowMs() const {
  return millis();
}
