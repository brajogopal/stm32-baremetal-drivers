# 08_HexBoot_F0

## Objective

Build a firmware transport layer from scratch using STM32F030 register-level programming.

This project receives firmware data over UART, validates the packet structure, reconstructs binary data into halfwords, and stores the received firmware into Flash memory.

---

## Learning Goals

- UART binary data reception
- UART timeout handling
- Transport header implementation
- Variable-length firmware transfer
- Halfword reconstruction
- Flash storage integration
- Protocol design fundamentals

---

## Packet Format

Current transport packet:

HEADER (1 Byte)
LENGTH (2 Bytes)
PAYLOAD (Variable Length)

Example:

AA 08 00 11 22 33 44 55 66 77 88

Interpretation:

Header = 0xAA

Length = 8 Bytes

Payload:

11 22 33 44 55 66 77 88

---

## Data Reconstruction

UART Payload:

11 22 33 44 55 66 77 88

Reconstructed Halfwords:

0x2211
0x4433
0x6655
0x8877

Reconstruction Logic:

```c
return (low | (high << 8));
```

---

## System Flow

Power On

↓

Wait For Header

↓

Validate Header

↓

Receive Payload Length

↓

Validate Length

↓

Convert Bytes To Halfwords

↓

Receive Firmware Payload

↓

Store In Flash

↓

Update Complete

---

## Implemented Features

UART Layer

- UART byte reception
- UART timeout protection
- Binary data transfer

Transport Layer

- Header detection
- Header validation
- Variable-length packet support

Firmware Layer

- Halfword reconstruction
- Firmware chunk reception
- Payload validation

Flash Layer

- Flash unlock
- Flash erase
- Flash program
- Flash verification

---

## Timeout Protection

Implemented:

```c
uart_receive_with_timeout()
```

Purpose:

- Prevent infinite blocking
- Detect communication failures
- Allow bootloader recovery

---

## Flash Driver Validation

Erase Verification

PASS

Buffer Programming

PASS

Alignment Protection

PASS

Locked Flash Protection

PASS

PGERR Investigation

PASS

---

## Transport Validation

Header Reception

PASS

Length Reception

PASS

Variable Length Transfer

PASS

Firmware Reconstruction

PASS

Flash Storage

PASS

---

## Challenges Faced

Length Definition Ambiguity

Question:

Does Length represent:

- Bytes?
- Halfwords?

Decision:

Length = Payload Size In Bytes

Reason:

Most communication protocols define payload size using bytes.

Examples:

- UART protocols
- Ethernet
- USB
- CAN
- TCP/IP

---

RealTerm Binary Transfer

Issue:

Transfer stopped unexpectedly.

Root Cause:

Transmission timing issue.

Solution:

Added transmission delay in RealTerm.

---

## Lessons Learned

- Protocol fields must be clearly defined.
- Payload length should use bytes.
- UART timeout handling is essential.
- Binary transfers differ from ASCII transfers.
- Transport framing simplifies firmware reception.
- Debug prints greatly accelerate embedded debugging.

---

## Current Status

UART Reception                  PASS

UART Timeout                    PASS

Transport Header                PASS

Length Field                    PASS

Variable Length Transfer        PASS

Halfword Reconstruction         PASS

Flash Storage                   PASS

---

## Next Phase

CRC Validation

Planned Packet Structure:

HEADER

↓

LENGTH

↓

PAYLOAD

↓

CRC

Goals:

- Firmware integrity verification
- Corruption detection
- Safe firmware updates
- Improved bootloader reliability

---

## Project Status

Project: 08_HexBoot_F0

Milestone:

Header + Variable Length Firmware Receiver

Status:

FROZEN