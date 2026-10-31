#include <unity.h>

#include "servo_sweep.h"

namespace {

class FakeClock final : public IClock {
 public:
  uint32_t nowMs() const override { return now; }
  uint32_t now = 0;
};

class FakeOutput final : public IAngleOutput {
 public:
  void setAngle(int angle) override {
    last = angle;
    ++calls;
  }
  int last = -1;
  int calls = 0;
};

FakeClock clock_;
FakeOutput output;
const SweepConfig kConfig{45, 48, 5, 100};

void advance(ServoSweep& sweep, uint32_t ms) {
  clock_.now += ms;
  sweep.update();
}

}  // namespace

void setUp() {
  clock_ = FakeClock();
  output = FakeOutput();
}

void tearDown() {}

void test_begin_moves_to_start_angle() {
  ServoSweep sweep(output, clock_, kConfig);
  TEST_ASSERT_TRUE(sweep.begin());
  TEST_ASSERT_EQUAL_INT(45, output.last);
}

void test_does_not_step_before_interval() {
  ServoSweep sweep(output, clock_, kConfig);
  sweep.begin();
  sweep.update();
  const int callsAfterFirstStep = output.calls;
  advance(sweep, 4);
  TEST_ASSERT_EQUAL_INT(callsAfterFirstStep, output.calls);
}

void test_reverses_at_endpoint_after_pause() {
  ServoSweep sweep(output, clock_, kConfig);
  sweep.begin();
  for (int i = 0; i < 3; ++i) {
    advance(sweep, 5);
  }
  TEST_ASSERT_EQUAL_INT(48, output.last);
  advance(sweep, 99);
  TEST_ASSERT_EQUAL_INT(48, output.last);
  advance(sweep, 1);
  TEST_ASSERT_EQUAL_INT(47, output.last);
}

void test_works_across_millis_overflow() {
  clock_.now = 0xFFFFFFF0u;
  ServoSweep sweep(output, clock_, kConfig);
  sweep.begin();
  advance(sweep, 5);
  TEST_ASSERT_EQUAL_INT(46, output.last);
  advance(sweep, 20);
  TEST_ASSERT_TRUE(clock_.now < 0x100u);
  TEST_ASSERT_EQUAL_INT(47, output.last);
}

void test_rejects_invalid_config() {
  const SweepConfig sameAngles{45, 45, 5, 0};
  const SweepConfig outOfRange{45, 200, 5, 0};
  const SweepConfig zeroInterval{45, 90, 0, 0};
  ServoSweep a(output, clock_, sameAngles);
  ServoSweep b(output, clock_, outOfRange);
  ServoSweep c(output, clock_, zeroInterval);
  TEST_ASSERT_FALSE(a.begin());
  TEST_ASSERT_FALSE(b.begin());
  TEST_ASSERT_FALSE(c.begin());
  TEST_ASSERT_EQUAL_INT(0, output.calls);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_begin_moves_to_start_angle);
  RUN_TEST(test_does_not_step_before_interval);
  RUN_TEST(test_reverses_at_endpoint_after_pause);
  RUN_TEST(test_works_across_millis_overflow);
  RUN_TEST(test_rejects_invalid_config);
  return UNITY_END();
}
