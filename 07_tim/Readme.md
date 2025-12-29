# STM32 Timers – Register Level Learning (TIM)

This folder contains **register-level STM32 timer examples** built step by step,
starting from basic SysTick usage to advanced timer features like
**Output Compare and Input Capture**.

All examples are written **without HAL**, using **direct register access**
on the **STM32F030C8T6** microcontroller.

The goal of this folder is to help learners understand:
- How timers work internally
- How to read the reference manual correctly
- How to build reusable, bare-metal drivers

---



Each folder represents **one learning stage**.

---

## 01️⃣ 01_systick_timer

### What you learn
- What SysTick is
- How the SysTick down-counter works
- Blocking delay using SysTick
- COUNTFLAG behavior
- Difference between LOAD, VAL, and CTRL registers

### Key concepts
- Cortex-M0 core timer
- Millisecond delays
- Write-to-clear registers

📌 Best for understanding **basic timing and delays**.

---

## 02️⃣ 02_basic_timer

### What you learn
- Using a general-purpose timer as a time base
- Prescaler and auto-reload calculation
- Update event (UIF)
- Difference between SysTick and peripheral timers

### Key concepts
- Timer clock tree (APB)
- PSC and ARR loading
- EGR (Update Generation)
- UIF flag handling

📌 Best for understanding **peripheral timers vs SysTick**.

---

## 03️⃣ 03_OutputCompare

### What you learn
- Output Compare (toggle mode)
- Input Capture
- Using two timers together
- Advanced timer concepts

### Implemented features
- **TIM1_CH1 (PA8)** → Output Compare (1 Hz toggle)
- **TIM3_CH1 (PA6)** → Input Capture
- External loopback: `PA8 → PA6`
- Explicit flag handling (CC1IF)
- Advanced timer requirements (MOE, BDTR)

### Key concepts
- CCxS input/output selection
- TI1 / TI2 internal signals
- CCR registers
- CCER, CCMR, SR behavior
- Why CCxE must be disabled before changing CCxS
- Difference between TIM1 (advanced) and TIM3 (general)

📌 Best for understanding **real-world timer applications**.

---

## 🧠 Learning Philosophy

- No HAL
- No CubeMX code generation
- Only **reference manual + CMSIS headers**
- Every register write is intentional
- Flags are handled explicitly

This approach builds **strong embedded fundamentals** and prepares you for:
- Driver development
- RTOS timers
- Low-level debugging
- Interview-level understanding

---


## 📎 Reference Documents

- STM32F030 Reference Manual (RM0360)
- ARM Cortex-M0 Technical Reference Manual
- STM32 CMSIS Device Headers

---

## ✍️ Author

Created and maintained by **Brajo**  
(Register-level STM32 learning series)
