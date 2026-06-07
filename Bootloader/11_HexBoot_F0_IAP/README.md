# 10_HexBoot_F0_FirmwareValidation

## Overview

This project extends the CRC-based firmware update mechanism developed in Project 09 by introducing firmware metadata storage and verification.

After successful firmware reception and CRC validation, the bootloader generates metadata containing firmware information and stores it in Flash memory. The metadata is then read back and verified through UART output.

---

## Features

* UART firmware reception
* CRC16 verification
* Firmware storage in Flash
* Metadata generation
* Metadata storage in Flash
* Metadata readback from Flash
* Flash operation status reporting

---

## Metadata Structure

```c
typedef struct
{
    uint32_t magic_number;
    uint32_t firmware_length;
    uint16_t firmware_crc;
    uint16_t reserved;
} firmware_metadata_t;
```

### Metadata Fields

| Field           | Description                                  |
| --------------- | -------------------------------------------- |
| magic_number    | Application identification value (APP_MAGIC) |
| firmware_length | Firmware size in bytes                       |
| firmware_crc    | CRC16 value of the firmware                  |
| reserved        | Reserved for future use                      |

---

## Flash Layout

```text
0x08004000 : Metadata
0x08004020 : Application
```

---

## Validation Flow

```text
Receive Firmware
        ↓
Calculate CRC
        ↓
CRC Match ?
   ↓         ↓
 YES         NO
  ↓           ↓
Store      Reject
Firmware
  ↓
Create Metadata
  ↓
Store Metadata
  ↓
Read Metadata
  ↓
Print Metadata
```

---

## Test Payload

```text
11 22 33 44 55 66 77 88
```

### Expected Results

```text
Firmware Length : 8 Bytes
Firmware CRC    : 0x5DB5
APP_MAGIC       : 0xDEADBEEF
```

---

## Project Output

Metadata successfully stored and read back from Flash:

```text
magic_number    : 0xDEADBEEF
firmware_length : 0x00000008
firmware_crc    : 0x5DB5
```

---

## Screenshots

### 01_Metadata_Readback_Verification.png

Demonstrates:

* Firmware reception
* CRC verification
* Metadata storage
* Metadata readback

### 02_Metadata_And_Firmware_In_Flash.png

Demonstrates:

* Metadata stored in Flash
* Firmware stored in Flash
* Correct memory layout

---

## Limitations

* Metadata validation is demonstrated through Flash readback.
* Boot-time application validation is not implemented in this project.
* Application jump logic is not integrated in this project.
* Single application slot architecture.

---

## Next Project

11_HexBoot_F0_IAP

Planned additions:

* Boot-time application validation
* Application jump logic
* In-Application Programming (IAP)
* Firmware update workflow integration
