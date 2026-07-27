#pragma once

#include "clock.h"

class ArduinoClock final : public IClock {
 public:
  uint32_t nowMs() const override;
};
