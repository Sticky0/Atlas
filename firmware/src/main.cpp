#include "arduino_clock.h"
#include "esp32_servo_output.h"
#include "servo_sweep.h"

namespace {

constexpr ServoHardwareConfig kCoxaServoConfig{
    18,     // GPIO pin
    50,     // PWM frequency in Hz
    1000,   // Minimum pulse width in microseconds
    2000};  // Maximum pulse width in microseconds

constexpr SweepConfig kCoxaSweepConfig{
    45,   // Start angle
    135,  // End angle
    20,   // Delay between angle steps in milliseconds
    500}; // Pause at each endpoint in milliseconds

ArduinoClock systemClock;
Esp32ServoOutput coxaServo(kCoxaServoConfig);
ServoSweep coxaSweep(coxaServo, systemClock, kCoxaSweepConfig);
bool systemReady = false;

}  // namespace

void setup() {
  systemReady = coxaServo.begin() && coxaSweep.begin();
}

void loop() {
  if (systemReady) {
    coxaSweep.update();
  }
}
