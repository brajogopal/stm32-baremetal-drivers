# STM32 Flash Metadata Storage System

## Overview

This project is a bare-metal STM32 firmware project developed on the **STM32F030C8T6** microcontroller using **register-level programming** without HAL or external libraries.

The goal of this project is to build a small but structured **persistent flash storage system** with UART-based interaction and metadata management.

The project demonstrates:

- Internal Flash programming
- Persistent data storage
- UART command interface
- Runtime metadata update
- Flash verification
- Defensive programming
- Layered firmware architecture

---

# Features

## Flash Driver

Implemented fully using register-level programming.

### Supported Operations

- Flash Unlock
- Flash Lock
- Page Erase
- Halfword Programming
- Buffer Programming
- Busy Wait Handling
- Timeout Protection
- Error Handling
- Write Protection Detection
- Programming Error Detection
- Flash Verification

---

## Persistent Metadata Storage

The system stores firmware-related metadata inside the last Flash page.

### Stored Metadata

```c
typedef struct
{
    uint32_t magic_number;
    uint32_t boot_count;
    uint32_t firmware_version;

} storage_data_t;
```

---

## UART Command Interface

Interactive UART-based metadata management system.

### Supported Commands

| Command | Description |
|---|---|
| `V` | View stored metadata |
| `U` | Update metadata |

---

## UART String Parsing

Implemented custom UART string receive function:

```c
void uart_receive_string(char *buffer,
                         uint32_t max_length);
```

### Features

- Character-by-character reception
- Echo support
- ENTER key detection
- Null termination
- Buffer overflow protection

---

# Flash Memory Layout

| Region | Address |
|---|---|
| Flash Start | `0x08000000` |
| Flash Size | `16 KB` |
| Page Size | `1 KB` |
| Storage Page | `0x0800FC00` |

The last Flash page is reserved for metadata storage.

---

# Project Architecture

## Layered Design

### 1. Flash Driver Layer

Files:
- `flash_driver.c`
- `flash_driver.h`

Responsibilities:
- Low-level Flash operations
- Error handling
- Verification
- Hardware interaction

---

### 2. Storage Manager Layer

Files:
- `storage_manager.c`
- `storage_manager.h`

Responsibilities:
- Metadata save/load abstraction
- Flash page management
- Persistent storage handling

---

### 3. UART Driver Layer

Files:
- `uart.c`
- `uart.h`

Responsibilities:
- UART TX/RX
- String input
- Debug printing

---

### 4. Application Layer

Files:
- `main.c`

Responsibilities:
- Menu system
- Metadata update flow
- User interaction
- Validation logic

---

# Metadata Update Flow

```text
Load Metadata From Flash
        ↓
Display Menu
        ↓
Receive User Command
        ↓
Authenticate Using Magic Number
        ↓
Receive New Metadata
        ↓
Update RAM Structure
        ↓
Save To Flash
        ↓
Reload From Flash
        ↓
Display Updated Metadata
```

---

# Defensive Features Implemented

- Flash alignment checking
- Erase verification
- Programming verification
- Flash lock protection
- Timeout handling
- Buffer overflow prevention
- Persistent data reload verification

---

# Technologies Used

- STM32F030C8T6
- Bare-metal Embedded C
- Register-Level Programming
- UART Communication
- Internal Flash Memory
- GCC Toolchain
- STM32CubeIDE

---

# Learning Outcomes

This project helped in understanding:

- STM32 Flash architecture
- Persistent storage systems
- UART communication flow
- ASCII to integer parsing
- Defensive firmware programming
- Pointer and buffer handling
- Layered firmware architecture
- Embedded system data flow

---

# Future Improvements

Planned future upgrades:

- CRC validation
- Firmware packet transfer
- Bootloader integration
- Circular UART buffers
- Interrupt-driven UART
- Firmware image validation
- Version compatibility checks

---

# Author

Brajo

Bare-metal STM32 learning journey toward advanced embedded systems, bootloader development, RTOS, and future SoC/VLSI design.