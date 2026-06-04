# 09_HexBoot_F0_CRC

STM32F030C8T6 UART Bootloader Project

## Objective

Add CRC16 validation to the bootloader and ensure firmware is programmed into Flash only when the received firmware passes CRC verification.

---

## Features

* UART firmware transfer
* Header validation
* Payload length validation
* Byte-buffer based firmware reception
* CRC16 verification
* Flash programming only after successful CRC validation
* Corrupted firmware detection

---

## CRC Configuration

| Parameter     | Value     |
| ------------- | --------- |
| CRC Type      | CRC16     |
| Polynomial    | 0x1021    |
| Initial Value | 0xFFFF    |
| Direction     | MSB First |

---

## Packet Format

| Field          | Size     |
| -------------- | -------- |
| Header         | 1 Byte   |
| Payload Length | 2 Bytes  |
| Payload Data   | Variable |
| CRC16          | 2 Bytes  |

```text
+--------+-----------+-------------+--------+
| Header | Length    | Payload     | CRC16  |
+--------+-----------+-------------+--------+
```

---

## CRC Validation Flow

```text
Receive Header
      ↓
Receive Length
      ↓
Receive Payload
      ↓
Receive CRC16
      ↓
Calculate CRC16
      ↓
Compare CRC Values
      ↓
Match ?
  /      \
Yes       No
 |         |
Program    Reject
Flash      Firmware
```

---

## CRC Implementation

```c
crc ^= ((uint16_t)data[i] << 8);

if(crc & 0x8000)
{
    crc <<= 1;
    crc ^= CRC16_POLY;
}
else
{
    crc <<= 1;
}
```

---

## Test Files

### Valid Firmware

File:

```text
test_files/CRC_Test_Valid.bin
```

Expected Result:

```text
CRC verified successfully
Firmware Update successful
```

---

### Corrupted Firmware

File:

```text
test_files/CRC_Test_Invalid.bin
```

Expected Result:

```text
CRC verification failed
corrupted data
restart the process
```

---

## Screenshots

### CRC Verification Success

```text
screenshots/crc_success.png
```

### CRC Verification Failure

```text
screenshots/crc_failure.png
```

---

## Project Structure

```text
09_HexBoot_F0_CRC
│
├── Inc
├── Src
├── Includes
├── Startup
├── Debug
│
├── screenshots
│   ├── crc_success.png
│   └── crc_failure.png
│
├── test_files
│   ├── CRC_Test_Valid.bin
│   └── CRC_Test_Invalid.bin
│
├── notes.txt
└── README.md
```

---

## Key Learning Outcomes

* CRC16 theory
* Polynomial-based error detection
* MSB-first CRC implementation
* Bitwise operations and shift registers
* Firmware integrity validation
* Secure Flash programming workflow

---

## Status

Project Frozen

Next Project:

```text
10_HexBoot_F0_FirmwareValidation
```
