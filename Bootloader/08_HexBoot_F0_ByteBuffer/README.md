# 08_HexBoot_F0_ByteBuffer

## Overview

This project migrates the firmware transport layer from a halfword-oriented design to a byte-oriented design.

The previous implementation reconstructed UART data into uint16_t values using reconstruct_halfword(). This version stores incoming firmware directly as bytes, making the architecture more suitable for future CRC validation and real firmware image transfers.

---

## Features

* UART Firmware Reception
* Transport Header Validation (0xAA)
* Variable Length Payload
* UART Receive Timeout
* Byte-Oriented Firmware Buffer
* Flash Programming
* Payload Length Validation

---

## Packet Format

+--------+--------+---------+
| HEADER | LENGTH | PAYLOAD |
+--------+--------+---------+

HEADER = 0xAA

LENGTH = Payload Length in Bytes

PAYLOAD = Firmware Data

---

## Architecture

PC (RealTerm)
|
v
UART Receive
|
v
uint8_t firmware_buffer[]
|
v
CRC Ready Architecture
|
v
(uint16_t*) Cast
|
v
Flash Programming

---

## Test Packet

AA
08 00
11 22 33 44 55 66 77 88

---

## Test Result

UART Output:

Firmware Receive successful

Firmware Update successful

Flash Memory:

11 22 33 44 55 66 77 88

stored successfully at:

0x0800F800

---

## Key Learning

* Firmware files are byte streams.
* UART transport should operate on bytes.
* Flash programming requirements should remain isolated inside the Flash layer.
* Type casting can be used to interpret the same memory region as uint8_t or uint16_t.
* This architecture prepares the bootloader for CRC validation.

---

## Screenshots

* uart_success.png
* flash_memory.png

---

## Next Step

09_HexBoot_F0_CRC

Implement CRC16 generation and validation before accepting firmware updates.
