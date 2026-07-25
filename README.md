# STM32 Bare-Metal Drivers

A collection of STM32 bare-metal drivers and embedded projects developed from scratch using CMSIS, without HAL or CubeMX.

> The goal of this repository is not just to provide reusable drivers, but to document the engineering process of learning STM32 through register-level programming.

---

## Repository Structure

```text
01_GPIO          GPIO Driver
02_UART          UART Driver
03_ADC           ADC Driver
04_I2C           I2C Driver
05_SPI           SPI Driver
07_TIM           Timer Driver
08_Interrupts    External Interrupts

Bootloader/      Bootloader Development (18 Projects)

chip_headers/    CMSIS Device Headers
docs/            Repository Documentation
```

---

## Learning Journey

### Peripheral Drivers

The first projects focus on understanding STM32 peripherals through register-level programming.

- GPIO
- UART
- ADC
- I2C
- SPI
- Timers
- Interrupts

---

### Bootloader Development

The **Bootloader** folder contains **18 projects (Project 00 – Project 17)** documenting the complete bootloader development process.

Topics include:

- Linker Scripts
- Flash Programming
- Vector Table Relocation
- UART Firmware Update
- CRC Verification
- State Machine Design
- Ping-Pong Buffer
- DMA
- Metadata Management
- Slot Management
- Failsafe Dual-Slot Bootloader

See:

```text
Bootloader/
```

---

## Repository Philosophy

- CMSIS Only
- No HAL
- No CubeMX
- Register-Level Programming
- Focus on understanding the hardware

---

## Future Roadmap

- STM32H7
- RTOS
- Embedded Linux

---

## License

MIT License