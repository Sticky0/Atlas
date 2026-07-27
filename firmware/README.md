# Atlas Firmware

The Atlas firmware is a PlatformIO project for an ESP32. The current firmware validates one coxa servo before expanding to single-leg and full-robot control.

## Current behavior

- Drives one servo from GPIO 18.
- Uses 50 Hz PWM and 1000–2000 µs pulse widths.
- Sweeps between 45° and 135°.
- Moves one degree every 20 ms.
- Pauses for 500 ms at each endpoint.
- Uses a non-blocking update loop; no `delay()` is used for motion.

## Build

From the repository root:

```bash
cd firmware
pio run
```

On Windows, if PlatformIO is not available in `PATH`:

```bash
cd firmware
/c/Users/isaac/.platformio/penv/Scripts/pio.exe run
```

The Wokwi simulation uses the generated files in:

```text
.pio/build/esp32dev/firmware.elf
.pio/build/esp32dev/firmware.bin
```

## Architecture

The source is divided by responsibility instead of keeping all behavior in `main.cpp`.

```text
firmware/
├── include/
│   ├── angle_output.h          # Angle output abstraction
│   ├── arduino_clock.h         # Arduino clock adapter
│   ├── clock.h                 # Clock abstraction
│   ├── esp32_servo_output.h    # ESP32Servo adapter
│   ├── servo_config.h          # Hardware and sweep configuration types
│   └── servo_sweep.h            # Sweep controller interface
└── src/
    ├── arduino_clock.cpp       # millis() implementation
    ├── esp32_servo_output.cpp   # Servo attachment and angle output
    ├── main.cpp                # Application composition and lifecycle
    └── servo_sweep.cpp          # Non-blocking sweep behavior
```

### SOLID principles applied

- **Single Responsibility:** hardware output, time access, sweep logic, and application composition have separate responsibilities.
- **Open/Closed:** new angle-output implementations, such as a PCA9685 adapter, can be added without modifying `ServoSweep`.
- **Liskov Substitution:** any implementation of `IAngleOutput` can be used by `ServoSweep`.
- **Interface Segregation:** `IAngleOutput` exposes only the operation required by the motion controller.
- **Dependency Inversion:** `ServoSweep` depends on `IAngleOutput` and `IClock`, not on `ESP32Servo` or the global `millis()` function.

The interfaces also make the sweep controller easier to test with fake output and clock implementations.

## Configuration

The hardware and motion settings are defined in `src/main.cpp`:

```cpp
constexpr ServoHardwareConfig kCoxaServoConfig{
    18, 50, 1000, 2000};

constexpr SweepConfig kCoxaSweepConfig{
    45, 135, 20, 500};
```

`ServoSweep::begin()` validates angle limits and timing values before starting the motion.

## Hardware warning

For real hardware, power servos from a dedicated supply. Do not power multiple servos directly from the ESP32. The ESP32 and servo supply must share a common ground.
