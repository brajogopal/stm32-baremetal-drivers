# Project 13: UART Interrupt Firmware Receiver

## Overview

Project 13 introduces interrupt-driven UART firmware reception for the HexBoot_F0 bootloader.

Unlike previous implementations that relied on polling, this project uses UART receive interrupts to asynchronously receive firmware data and store it into RAM before programming it into Flash memory.

This project represents the transition from peripheral-level programming to embedded system design concepts such as interrupt handling, chunked data transfer, firmware protocols, and bootloader architecture.

---

## Features

* UART interrupt-driven firmware reception
* Chunk-based Flash programming
* CRC16-CCITT firmware verification
* Metadata generation and application validation
* Automatic jump to application after successful update
* Support for partial final firmware chunks
* Firmware packet protocol with header and length validation

---

## Firmware Packet Format

The bootloader expects firmware packets in the following format:

```text
+--------+----------+--------+-------------+
| Header | Length   | CRC16  | Payload     |
+--------+----------+--------+-------------+

Header  : 1 Byte
Length  : 2 Bytes (Little Endian)
CRC16   : 2 Bytes (Little Endian)
Payload : Firmware Binary
```

### Example

```text
AA 2C 02 D0 E4 <Firmware Data ...>
```

where:

* `AA` → Header
* `0x022C` → Firmware length (556 bytes)
* `0xE4D0` → CRC16-CCITT

---

## Bootloader Flow

```text
Receive Header (Polling)
            ↓
Validate Header
            ↓
Receive Payload Length (Polling)
            ↓
Receive Expected CRC16 (Polling)
            ↓
Validate Firmware Size
            ↓
Erase Flash Region
            ↓
Enable UART Interrupt
            ↓
Receive Firmware Chunks
            ↓
Program Flash
            ↓
Receive Final Chunk
            ↓
Calculate CRC over Flash
            ↓
CRC Match?
      ↓ Yes           ↓ No
Update Metadata     Reject Firmware
      ↓
Jump To Application
```

---

## UART Interrupt Architecture

```text
PC UART Sender
       ↓
USART2 RX Interrupt
       ↓
RAM Buffer (128 Bytes)
       ↓
Chunk Ready Flag
       ↓
Flash Programming
```

---

## Chunk-Based Reception

Firmware is received in fixed-size chunks:

```text
CHUNK_SIZE = 128 Bytes
```

Each chunk is programmed to Flash immediately after reception.

The final chunk may be smaller than 128 bytes and is automatically handled using the received payload length.

---

## CRC Verification

CRC Algorithm:

```text
CRC16-CCITT
Polynomial : 0x1021
Initial CRC: 0xFFFF
```

The CRC is calculated over the programmed firmware stored in Flash memory.

This verifies:

* UART transmission integrity
* Flash programming integrity

---

## Memory Layout

```text
Bootloader Region
┌──────────────────────┐
│ Bootloader           │
├──────────────────────┤
│ Metadata             │
├──────────────────────┤
│ Application A        │
│ Firmware Image       │
└──────────────────────┘
```

---

## Design Decisions

### Interrupt-Driven Payload Reception

Small metadata fields are received using blocking UART:

* Header
* Payload Length
* CRC16

Firmware payload is received using UART interrupts.

This keeps the interrupt path dedicated to high-volume data transfer.

---

## Known Limitations

This project intentionally uses a single receive buffer.

During Flash programming:

```c
__disable_irq();

/* Flash programming */

__enable_irq();
```

Interrupts are temporarily disabled to prevent UART data from overwriting the active receive buffer.

### Limitation

Continuous high-speed streaming is not supported in this version.

UART reception pauses while Flash is being programmed.

---

## Future Improvements

### Project 14

* Ping-Pong (Double) Buffering
* Continuous UART Reception
* Non-blocking Flash Programming

### Project 15

* DMA-Based Reception
* ACK/NACK Flow Control
* Advanced Packet Protocol
* Sequence Numbers
* Error Recovery

---

## Learning Outcomes

This project demonstrates:

* UART Interrupt Handling
* ISR Design
* Critical Sections
* Chunked Data Transfer
* Flash Programming
* Firmware Verification
* Bootloader Architecture
* Embedded System Design

---

## Test Environment

* MCU: STM32F030C8T6
* UART: USART2
* Baud Rate: 9600
* Tool: RealTerm
* Packet Generator: Python

---

## Repository

Part of the STM32 Bare-Metal Drivers and HexBoot_F0 learning ecosystem.

Author: Brajo
Year: 2026

> "I will climb all the mountain in my path to success."
