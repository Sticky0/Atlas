# 🤖 Atlas

A 3D-printed hexapod robot, designed and built from scratch: mechanics, electronics, and firmware.

## 📋 Description

Atlas is a hexapod robot with 6 legs and 3 degrees of freedom per leg: coxa, femur, and tibia. That gives the robot 18 servos in total.

The project is being developed step by step. The current firmware validates basic servo control on an ESP32 before moving to full single-leg inverse kinematics and, later, complete gait control.

## 🚀 Current status

Current phase: **single-servo validation**.

The firmware currently drives one coxa servo connected to GPIO 18. The servo sweeps smoothly between 45° and 135°, using a standard 50 Hz servo PWM signal.

## 🛠️ Hardware

### 3D Printing

| Stage | Material | Reason |
|-------|----------|--------|
| Prototyping | PLA | Fast, cheap, and easy to print while iterating on geometry |
| Final build | ASA | Higher mechanical, thermal, and UV resistance |

### Electronics

- **Microcontroller:** ESP32
- **Current test servo:** SG90 9G Micro Servo
- **Future servo driver:** PCA9685 PWM driver over I2C
- **Final build servos:** MG996R
- **Power supply:** dedicated power source for the servos, separated from the ESP32 power input, with common ground

### Current wiring

| Servo wire | ESP32 |
|------------|-------|
| Signal | GPIO 18 |
| V+ | 5V |
| GND | GND |

> For real hardware, use an external servo power supply when the load increases. Do not power multiple servos directly from the ESP32.

## 💻 Firmware

The firmware is a PlatformIO project located in:

```text
cplusplus/
```

Build it with:

```bash
cd cplusplus
pio run
```

If `pio` is not in your PATH on Windows, use:

```bash
cd cplusplus
/c/Users/isaac/.platformio/penv/Scripts/pio.exe run
```

## 📁 Project structure

```text
atlas/
├── cplusplus/
│   ├── src/
│   │   └── main.cpp          # ESP32 servo test firmware
│   ├── platformio.ini        # PlatformIO configuration
│   ├── diagram.json          # Wokwi simulation wiring
│   └── wokwi.toml            # Wokwi configuration
├── LICENSE
└── README.md
```

## 🗺️ Roadmap

- [x] Basic ESP32 servo control
- [x] Wokwi single-servo simulation
- [ ] Single-leg mechanical prototype in PLA + SG90
- [ ] Inverse kinematics for one leg, 3 DOF
- [ ] Range-of-motion validation
- [ ] PCA9685 integration
- [ ] Design and print all 6 legs
- [ ] Full chassis assembly
- [ ] Gait implementation: tripod and wave
- [ ] Migration to MG996R and ASA printed parts
- [ ] Stability and power-consumption tuning

## 📄 License

AGPL-3.0 license. See [LICENSE](./LICENSE).
