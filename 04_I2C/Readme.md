# 📘 STM32F0 I2C Driver (Register-Level, Bare-Metal)

## Overview

This project implements a **fully register-level I2C master driver** for **STM32F0 (STM32F030x8)** microcontrollers **without using STM32 HAL or CubeMX**.

The driver was built incrementally and validated using the **MPU6050 IMU**, covering real-world I2C use cases such as:

- GPIO alternate function configuration
- Open-drain I2C electrical behavior
- I2C timing calculation (`I2C_TIMINGR`)
- Single-byte and multi-byte register access
- Restart-based read sequences
- Debugging using I2C status flags
- Reading accelerometer, gyroscope, and temperature data

The goal of this implementation is **learning and understanding the STM32 I2C peripheral at silicon level**, not abstraction.

---

## Target Hardware

- **MCU**: STM32F030x8 (Cortex-M0)
- **Peripheral**: I2C1
- **Pins Used**:
  - `PB8` → I2C1_SCL (AF1)
  - `PB9` → I2C1_SDA (AF1)
- **Validation Device**: MPU6050 (I2C address `0x68`)

---

## GPIO Configuration for I2C

The GPIO pins are configured strictly according to the I2C specification:

- Alternate Function mode
- Open-drain output type
- Internal pull-up resistors enabled

```c
GPIOB->MODER  |= (2U << 16) | (2U << 18);   // AF mode
GPIOB->OTYPER |= (1U << 8)  | (1U << 9);    // Open-drain
GPIOB->PUPDR  |= (1U << 16) | (1U << 18);   // Pull-up

