# Project 17 - HexBoot_F0 Failsafe Dual-Slot Bootloader

A bare-metal dual-slot bootloader for the **STM32F030C8T6** written without **HAL** or **CubeMX**.

## Features

- Dual-slot firmware update
- UART + DMA firmware transfer
- Ping-Pong buffer
- CRC16 verification
- Metadata management
- Automatic slot selection
- Boot timeout
- Failsafe recovery

---

## Project Structure

![Project Structure](screenshots/01_Folder_tree.png)

---

## Bootloader

![Bootloader](screenshots/02_Bootloader_output.png)

---

## Slot A Update

![Slot A](screenshots/03_Slot_A_update.png)

---

## Slot B Update

![Slot B](screenshots/04_Slot_B_update.png)

---

## Memory Layout

![Memory Layout](screenshots/05_memory_layout.png)

---

## Boot Flow

![Boot Flow](screenshots/06_boot_flow.png)

---

## Software Architecture

![Architecture](screenshots/07_architecture.png)

---

## Firmware Update Flow

![Firmware Update](screenshots/08_firmware_update.png)

---

## Folder Structure

```text
Inc/            Header files
Src/            Source files
Startup/        Startup code
docs/           Documentation
screenshots/    Project images
test_files/     Test firmware
Tools/          Python utilities
```

---

## Documentation

- Architecture Diagram
- Boot Flow
- Firmware Update Flow
- Memory Layout

See the **docs/** folder.

---

## Tools

The **Tools/** folder contains the Python utility used to generate the firmware header.

---

## License

MIT License