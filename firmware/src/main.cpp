#include <Arduino.h>
#include <ESP32Servo.h>

namespace {

struct ServoConfig {
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

// Abstract output used by the motion controller.
class IAngleOutput {
 public:
  virtual ~IAngleOutput() = default;
  virtual void setAngle(int angle) = 0;
};

// Responsible only for configuring and driving the ESP32 servo hardware.
class Esp32ServoOutput final : public IAngleOutput {
 public:
  explicit Esp32ServoOutput(const ServoConfig& config) : config_(config) {}

  bool begin() {
    servo_.setPeriodHertz(config_.frequencyHz);
    attached_ = servo_.attach(
                    config_.pin,
                    config_.minPulseWidthUs,
                    config_.maxPulseWidthUs) > 0;
    return attached_;
  }

  void setAngle(const int angle) override {
    if (attached_) {
      servo_.write(angle);
    }
  }

 private:
  const ServoConfig& config_;
  Servo servo_;
  bool attached_ = false;
};

// Responsible only for generating a non-blocking back-and-forth movement.
class ServoSweep final {
 public:
  ServoSweep(IAngleOutput& output, const SweepConfig& config)
      : output_(output),
        config_(config),
        currentAngle_(config.startAngle),
        direction_(config.startAngle <= config.endAngle ? 1 : -1) {}

  void begin() {
    currentAngle_ = config_.startAngle;
    direction_ = config_.startAngle <= config_.endAngle ? 1 : -1;
    output_.setAngle(currentAngle_);
    nextStepAtMs_ = millis();
  }

  void update() {
    const uint32_t now = millis();
    if (static_cast<int32_t>(now - nextStepAtMs_) < 0) {
      return;
    }

    currentAngle_ += direction_;
    output_.setAngle(currentAngle_);

    if (currentAngle_ == config_.endAngle ||
        currentAngle_ == config_.startAngle) {
      direction_ = -direction_;
      nextStepAtMs_ = now + config_.endpointPauseMs;
      return;
    }

    nextStepAtMs_ = now + config_.stepIntervalMs;
  }

 private:
  IAngleOutput& output_;
  const SweepConfig& config_;
  int currentAngle_;
  int direction_;
  uint32_t nextStepAtMs_ = 0;
};

constexpr ServoConfig kCoxaServoConfig{
    18,     // GPIO pin
    50,     // PWM frequency in Hz
    1000,   // Minimum pulse width in microseconds
    2000};  // Maximum pulse width in microseconds

constexpr SweepConfig kCoxaSweepConfig{
    45,   // Start angle
    135,  // End angle
    20,   // Delay between angle steps in milliseconds
    500}; // Pause at each endpoint in milliseconds

Esp32ServoOutput coxaServo(kCoxaServoConfig);
ServoSweep coxaSweep(coxaServo, kCoxaSweepConfig);
bool servoReady = false;

}  // namespace

void setup() {
  servoReady = coxaServo.begin();
  if (servoReady) {
    coxaSweep.begin();
  }
}

void loop() {
  if (servoReady) {
    coxaSweep.update();
  }
}
