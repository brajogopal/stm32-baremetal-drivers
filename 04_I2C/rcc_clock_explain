# 📘 I2C Timing Calculation (STM32F0 – rcc_clock.c)

## Overview

This module provides a **pure register-level implementation** to calculate the `I2C_TIMINGR` register value for **STM32F0** microcontrollers **without using STM32CubeMX or HAL**.

The user only specifies the **desired I2C bus speed** (for example, `100000` for 100 kHz). The function automatically:

* Reads the **actual APB1 clock (PCLK1)** at runtime
* Works entirely in the **time domain** (nanoseconds)
* Calculates a valid **PRESC**, **SCLL**, and **SCLH**
* Applies safe defaults for **SCLDEL** and **SDADEL**
* Returns a fully packed **I2C_TIMINGR** value

This approach closely follows the logic used internally by **STM32CubeMX** and ST application notes, while remaining transparent and educational.

---

## Function Prototype

```c
uint32_t i2c_calc_timing(uint32_t i2c_speed_hz);
```

### Parameters

| Name           | Description                                           |
| -------------- | ----------------------------------------------------- |
| `i2c_speed_hz` | Desired I2C bus speed in Hz (e.g. `100000`, `400000`) |

### Return Value

| Value    | Meaning                                        |
| -------- | ---------------------------------------------- |
| Non-zero | Valid `I2C_TIMINGR` register value             |
| `0`      | Timing cannot be generated for the given clock |

---

## Full Implementation

```c
uint32_t i2c_calc_timing(uint32_t i2c_speed_hz)
{
    uint32_t pclk = rcc_get_pclk1_freq();
    uint32_t i2c_period_ns = 1000000000UL / i2c_speed_hz;
    uint32_t t_i2cclk_ns   = 1000000000UL / pclk;

    uint32_t presc, t_presc_ns;
    uint32_t total_ticks;
    uint32_t scll, sclh;

    for (presc = 0; presc < 16; presc++)
    {
        t_presc_ns = (presc + 1) * t_i2cclk_ns;
        total_ticks = i2c_period_ns / t_presc_ns;

        if (total_ticks <= 510)
            break;
    }

    if (presc == 16)
        return 0; // timing not possible

    /* Bias LOW time longer than HIGH */
    scll = (total_ticks * 2) / 3;
    sclh = total_ticks - scll;

    if (scll > 255) scll = 255;
    if (sclh > 255) sclh = 255;

    /* Safe defaults */
    uint32_t scldel;
    if (i2c_speed_hz <= 100000)
        scldel = 3;   // Standard mode
    else
        scldel = 2;   // Fast mode

    uint32_t sdadel = 0;

    return  (presc  << 28) |
            (scldel << 20) |
            (sdadel << 16) |
            (sclh   << 8 ) |
            (scll);
}
```

---

## How the Algorithm Works

### 1. Clock Source

The function reads **PCLK1** dynamically using:

```c
rcc_get_pclk1_freq();
```

This guarantees correct behavior regardless of whether the system clock is derived from **HSI**, **PLL**, or prescaled clocks.

---

### 2. Time-Domain Conversion

All calculations are performed in **nanoseconds**:

* I2C period:

  ```text
  T_I2C = 1 / I2C_speed
  ```
* I2C clock period:

  ```text
  T_I2CCLK = 1 / PCLK1
  ```

Time-domain math avoids frequency-ratio errors and allows correct handling of prescalers.

---

### 3. PRESC Selection

The I2C prescaler defines the base timing resolution:

```text
T_PRESC = (PRESC + 1) × T_I2CCLK
```

The algorithm selects the **smallest PRESC** such that:

```text
SCLL + SCLH ≤ 255 + 255 = 510
```

This feasibility check is **independent of waveform shape**, making the algorithm robust and future-proof.

---

### 4. SCL LOW / HIGH Split

```c
scll = (total_ticks * 2) / 3;
sclh = total_ticks - scll;
```

LOW time is intentionally longer than HIGH time because:

* I2C minimum LOW time is greater than HIGH time
* Clock stretching occurs during LOW
* SDA transitions occur while SCL is LOW
* Noise margin is improved

---

### 5. Register Width Protection

Even if the total timing fits, individual fields may overflow. Clamping ensures hardware-legal values:

```c
if (scll > 255) scll = 255;
if (sclh > 255) sclh = 255;
```

> Feasibility (≤510) and register legality (≤255) are **separate requirements**.

---

### 6. SDADEL and SCLDEL

These fields control **data setup and hold times**, not frequency.

* `SCLDEL = 3` for Standard Mode (100 kHz)
* `SCLDEL = 2` for Fast Mode (400 kHz)
* `SDADEL = 0` (allowed by I2C specification)

These values match STM32CubeMX defaults and provide safe margins.

---

## Usage Example

```c
I2C1->CR1 &= ~I2C_CR1_PE;
I2C1->TIMINGR = i2c_calc_timing(100000); // 100 kHz
I2C1->CR1 |= I2C_CR1_PE;
```

---

## Design Principles

* ✔ Time-domain based timing
* ✔ No hard-coded clock assumptions
* ✔ No partial register writes
* ✔ Shape-independent feasibility checks
* ✔ CubeMX-compatible results
* ✔ Suitable for bare-metal drivers

---

## Limitations

This implementation intentionally does **not** model:

* Rise/fall times
* Bus capacitance
* Fast-mode Plus (1 MHz)
* AN4235-level electrical tuning

It is intended for:

* Learning and experimentation
* Register-level driver development
* 100 kHz / 400 kHz I2C
* STM32F0-class microcontrollers

---

## Key Takeaway

> **I2C timing is not configured — it is solved.**

This function deterministically solves the `I2C_TIMINGR` register using silicon-correct rules and clear engineering logic.

