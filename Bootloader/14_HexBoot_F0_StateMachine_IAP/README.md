# Project 14 - HexBoot F0 State Machine IAP

## Overview

This project implements an interrupt-driven packet parser for firmware updates on the STM32F030C8T6.

Unlike Project 13, where firmware metadata was received using blocking functions and the payload was received using UART interrupts, this project introduces a packet parser state machine that processes the entire firmware packet through UART interrupt-driven reception.

The firmware update packet contains:

```text
HEADER | PAYLOAD_LENGTH | CRC16 | PAYLOAD
```

The parser reconstructs metadata, receives firmware payload chunks, programs flash memory, and verifies firmware integrity using CRC16.

---

## Features

* UART interrupt-driven firmware reception
* Packet parser state machine
* Header detection
* Payload length reconstruction
* CRC16 reconstruction
* Chunk-based flash programming
* Firmware integrity verification
* Metadata update support
* Bootloader compatible architecture

---

## Packet Parser State Machine

```text
WAIT_HEADER
    ↓
READ_LENGTH
    ↓
READ_CRC
    ↓
RECEIVE_PAYLOAD
    ↓
FW_COMPLETE
```

### State Description

| State           | Description                        |
| --------------- | ---------------------------------- |
| WAIT_HEADER     | Waits for packet start byte        |
| READ_LENGTH     | Receives firmware payload length   |
| READ_CRC        | Receives expected CRC16            |
| RECEIVE_PAYLOAD | Receives firmware payload          |
| FW_COMPLETE     | Firmware reception completed       |
| FW_ERROR        | Reserved for future error handling |

---

## Firmware Packet Format

```text
+---------+----------------+---------+-----------+
| Header  | Payload Length | CRC16   | Payload   |
+---------+----------------+---------+-----------+
| 1 Byte  | 2 Bytes        | 2 Bytes | N Bytes   |
+---------+----------------+---------+-----------+
```

---

## Test Result

Firmware Size:

* 556 Bytes

Chunk Size:

* 128 Bytes

Transfer Sequence:

```text
Chunk 1 : 128 Bytes
Chunk 2 : 128 Bytes
Chunk 3 : 128 Bytes
Chunk 4 : 128 Bytes
Chunk 5 : 44 Bytes
```

Verification:

```text
Received CRC   : 0xE4D0
Calculated CRC : 0xE4D0
CRC verified successfully
```

---

## Improvements Over Project 13

* Complete packet metadata received using UART interrupts
* Dedicated firmware receiver module
* Packet parser architecture
* Cleaner ISR design
* Improved project modularity
* Improved firmware reception flow

---

## Current Limitations

* Single receive buffer architecture
* UART interrupts disabled during flash programming
* Sender requires inter-chunk delay
* No ACK/NACK flow control
* No DMA support

---

## Future Improvements (Project 15 and later)

* Ping-Pong (Double Buffer) Reception
* Continuous reception during flash programming
* ACK/NACK protocol
* DMA-based UART reception
* Higher throughput firmware updates

---

## Learning Outcomes

* UART interrupt handling
* Packet parser implementation
* Embedded state machine design
* Flash memory programming
* CRC-based firmware validation
* Modular firmware architecture
