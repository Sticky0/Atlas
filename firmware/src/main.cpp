#include <Arduino.h>
#include <ESP32Servo.h>

constexpr uint8_t kServoPin = 18;
constexpr int kMinPulseWidthUs = 1000;
constexpr int kMaxPulseWidthUs = 2000;
constexpr int kStartAngle = 45;
constexpr int kEndAngle = 135;
constexpr int kStepDelayMs = 20;
constexpr int kEndpointDelayMs = 500;
constexpr uint32_t kSerialBaudRate = 115200;

Servo coxaServo;

void moveServo(const int fromAngle, const int toAngle) {
  const int step = (fromAngle < toAngle) ? 1 : -1;

  for (int angle = fromAngle; angle != toAngle; angle += step) {
    coxaServo.write(angle);
    delay(kStepDelayMs);
  }

  coxaServo.write(toAngle);
}

void setup() {
  Serial.begin(kSerialBaudRate);

  coxaServo.setPeriodHertz(50);
  coxaServo.attach(kServoPin, kMinPulseWidthUs, kMaxPulseWidthUs);
  coxaServo.write(kStartAngle);
}

void loop() {
  moveServo(kStartAngle, kEndAngle);
  delay(kEndpointDelayMs);

  moveServo(kEndAngle, kStartAngle);
  delay(kEndpointDelayMs);
}
