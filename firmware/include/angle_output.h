#pragma once

class IAngleOutput {
 public:
  virtual ~IAngleOutput() = default;
  virtual void setAngle(int angle) = 0;
};
