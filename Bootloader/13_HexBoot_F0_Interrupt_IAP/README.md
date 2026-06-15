# Project 12 - HexBoot F0 Streaming IAP Bootloader

## Overview

Project 12 introduces chunk-based firmware programming for the STM32F030 bootloader.

In previous versions, the entire firmware image was received before being programmed into Flash. In this project, firmware is received in fixed-size chunks and programmed immediately after reception.

This approach significantly reduces RAM usage and prepares the bootloader architecture for future packet-based firmware update protocols.

---

## Target MCU

* STM32F030C8T6
* Cortex-M0
* Register-Level Programming
* No HAL
* No CubeMX

---

## New Features Compared to Project 11

### Project 11

* Entire firmware received before programming
* Firmware stored in RAM before Flash programming
* Single programming operation

### Project 12

* Firmware received in 128-byte chunks
* Each chunk programmed immediately
* Dynamic Flash erase based on firmware size
* Flash verification after each chunk programming
* CRC calculated directly from Flash memory
* Reduced RAM usage
* Streaming firmware update architecture

---

## Memory Layout

### Flash Memory

| Region        | Address    |
| ------------- | ---------- |
| Bootloader    | 0x08000000 |
| Metadata      | 0x08004000 |
| Application A | 0x08004020 |


---

## Firmware Packet Format

| Field            | Size    |
| ---------------- | ------- |
| Header           | 1 Byte  |
| Firmware Length  | 2 Bytes |
| Firmware Payload | N Bytes |
| CRC16            | 2 Bytes |

### Header Value

```text
0xAA
```

---

## Firmware Update Flow

```text
Receive Header
        ↓
Receive Firmware Length
        ↓
Erase Required Flash Pages
        ↓
Receive Chunk
        ↓
Program Chunk
        ↓
Verify Chunk
        ↓
Repeat Until Firmware Complete
        ↓
Receive CRC16
        ↓
Calculate CRC16 From Flash
        ↓
Compare CRC Values
        ↓
Write Metadata
        ↓
Jump To Application
```

---

## Chunk Configuration

```c
#define CHUNK_SIZE 128
```

### Operation

```text
Receive 128 Bytes
        ↓
Program Flash
        ↓
Verify Flash
        ↓
Receive Next Chunk
```

The final chunk can be smaller than 128 bytes and is handled automatically.

---

## Flash Verification

After programming each chunk:

```text
Read Flash Data
        ↓
Compare With Received Buffer
        ↓
Pass / Fail
```

This ensures that programmed data matches received data before continuing.

---

## CRC Verification

CRC is calculated directly from Flash memory after the entire firmware image has been programmed.

Example:

```c
calculated_crc =
    crc16_calculate(
        (uint8_t*)APPLICATION_A_ADDRESS,
        payload_length_bytes);
```

This verifies the actual contents stored in Flash rather than only the received UART data.

---

## Metadata Structure

Metadata is written after successful CRC verification.

Stored Information:

* Application Magic Number
* Firmware Length
* Firmware CRC

Example:

```text
Magic Number    : 0xDEADBEEF
Firmware Length : 1792 Bytes
Firmware CRC    : 0x3548
```

---

## Test Results

### Test 1

Firmware Size:

```text
556 Bytes
```

Result:

```text
PASS
```

### Test 2

Firmware Size:

```text
1792 Bytes
```

Result:

```text
PASS
```

### Verification Results

```text
Flash Programming     PASS
Flash Verification    PASS
CRC Verification      PASS
Metadata Update       PASS
Application Boot      PASS
```

---

## Screenshots

Included in:

```text
screenshots/
```

Contains:

* Successful 556-byte firmware update
* Successful 1792-byte firmware update
* Chunk programming debug output
* Flash memory view
* Metadata verification

---

## Tools

Included in:

```text
Tools/
```

### packet_generator.py

Converts IDE-generated firmware binaries into bootloader packets.

Generated packet format:

```text
Header
Length
Payload
CRC16
```

---

## Known Limitation

Current implementation relies on sender-side transmission delay.

Large firmware transfers require sufficient inter-character delay because Flash programming and verification occur between chunk receptions.

Example:

```text
30 Delay  -> Transfer Failure
100 Delay -> Transfer Success
```

---

## Future Improvements

Project 13

* Packet-based firmware transfer
* Packet numbering
* Packet validation

Project 13.5

* ACK/NACK protocol
* Packet retransmission
* Error recovery

Project 14

* Double buffering
* Background Flash programming
* Higher throughput firmware updates

---

## Learning Outcomes

Through this project the following concepts were implemented and tested:

* Flash Memory Programming
* Dynamic Page Erase
* Firmware Streaming
* Chunk-Based Processing
* Memory-Mapped Flash Access
* Flash Verification
* CRC Verification
* Firmware Metadata Management
* Application Validation
* Bootloader to Application Jump

---

Author: Brajogopal Chakraborty

Development Style:

```text
STM32 Register-Level Programming
Bare-Metal Development
No HAL
No CubeMX
```
