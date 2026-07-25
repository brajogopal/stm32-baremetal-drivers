# STM32 Bootloader Development

A collection of bootloader projects developed for the STM32F030 using **CMSIS**, without **HAL** or **CubeMX**.

Each project focuses on a specific concept and builds upon the previous one, gradually evolving into a complete failsafe dual-slot bootloader.

---

## Development Roadmap

| Stage | Focus |
|--------|-------|
| 00 – 01 | System Setup & Linker Scripts |
| 02 – 07 | Bootloader Fundamentals |
| 08 – 10 | HexBoot_F0 Architecture |
| 11 – 17 | Firmware Update & Failsafe Bootloader |

---

## Project Progression

```text
System Driver
      │
      ▼
Linker Script
      │
      ▼
Basic Bootloader
      │
      ▼
Multi-Slot Bootloader
      │
      ▼
Flash Validation
      │
      ▼
HexBoot_F0
      │
      ▼
CRC
      │
      ▼
Streaming IAP
      │
      ▼
Interrupt IAP
      │
      ▼
State Machine
      │
      ▼
Ping-Pong Buffer
      │
      ▼
DMA
      │
      ▼
Failsafe Dual-Slot Bootloader
```

---

## Key Topics

- Flash Programming
- Linker Scripts
- Firmware Validation
- CRC16
- UART Communication
- Interrupts
- DMA
- Ping-Pong Buffer
- Metadata Management
- Slot Management
- Firmware Update (IAP)
- Failsafe Recovery

---

## Featured Project

⭐ **Project 17 – HexBoot_F0 Failsafe Dual-Slot Bootloader**

Features

- Dual-slot firmware update
- UART + DMA transfer
- Ping-Pong buffer
- CRC16 verification
- Metadata management
- Automatic slot selection
- Boot timeout
- Failsafe recovery

Project documentation is available inside the Project 17 folder.

```text
17_HexBoot_F0_Failsafe_Dualslot_Bootloader/
```

---

## Hardware

- STM32F030C8T6

---

## License

MIT License