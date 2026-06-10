# 11_HexBoot_F0_IAP

## Overview

This project implements a complete UART-based In-Application Programming (IAP) bootloader for the STM32F030C8T6 microcontroller using register-level programming.

The bootloader receives firmware packets through UART, validates firmware integrity using CRC16, programs the firmware into internal Flash memory, stores firmware metadata, validates the application, and transfers execution to the newly programmed application.

---

## Features

- UART Firmware Update
- Custom Firmware Packet Protocol
- CRC16 Verification
- Flash Programming
- Firmware Metadata Storage
- Application Validation
- Automatic Application Jump
- UART Timeout Recovery
- Multi-Page Flash Erase Support

---

## Firmware Packet Format

| Field | Size |
|---------|---------|
| Header | 1 Byte |
| Payload Length | 2 Bytes |
| Firmware Payload | N Bytes |
| CRC16 | 2 Bytes |

Packet Structure:

```text
+--------+----------+-------------+-------+
| Header | Length   | Payload     | CRC16 |
+--------+----------+-------------+-------+
```

---

## Memory Map

```text
0x08000000  Bootloader

0x08004000  Metadata

0x08004020  Application A
```

---

## Boot Flow

```text
Reset
 ↓
Bootloader Start
 ↓
Wait for Firmware Packet
 ↓
Timeout?
 ├─ Yes → Jump to Existing Application
 └─ No
       ↓
Receive Firmware
       ↓
CRC Verification
       ↓
Flash Programming
       ↓
Metadata Update
       ↓
Application Validation
       ↓
Jump to Application
```

---

## Test Results

### Firmware Update Successful

- CRC Verification Passed
- Flash Programming Successful
- Metadata Written Successfully
- Application Validation Successful
- Application Jump Successful

Screenshot:

`01_IAP_Successful_Firmware_Update.png`

---

### Application and Metadata Stored in Flash

Metadata contains:

- Magic Number
- Firmware Length
- Firmware CRC

Screenshot:

`02_Application_And_Metadata_In_Flash.png`

---

## Important Learning

During testing a firmware image larger than one flash page exposed a flash erase issue.

Root Cause:

```text
Only one flash page was erased before programming.
```

Solution:

```text
Calculate required pages dynamically and erase all pages before programming.
```

This added support for firmware images spanning multiple flash pages.

---

## Known Limitation

Current implementation stores the entire firmware image in RAM before programming.

```c
uint8_t firmware_buffer[2048];
```

---

## Future Work

Project 12 will implement:

- Chunk-Based Firmware Transfer
- Streaming Flash Programming
- Reduced RAM Usage

---

## Target MCU

STM32F030C8T6

---

## Development Style

Register-Level Programming (No HAL)
