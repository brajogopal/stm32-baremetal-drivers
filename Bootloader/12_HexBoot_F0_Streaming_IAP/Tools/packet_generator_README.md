# HexBoot Packet Generator

## Overview

This Python script converts a standard STM32 firmware binary (`.bin`) file into a firmware packet compatible with the HexBoot_F0 bootloader.

The generated packet can be transmitted through UART using tools such as RealTerm and received by the bootloader for firmware updates.

---

## Development Note

Initial version of this tool was created with assistance from ChatGPT and later reviewed, tested, and integrated into the HexBoot_F0 bootloader project by Brajo.

The script is included as a development utility to simplify firmware testing and bootloader validation.

---

## Packet Format

The generated packet follows the protocol implemented in Project 11 (HexBoot_F0_IAP).

```text
+--------+----------+-------------+-------+
| Header | Length   | Payload     | CRC16 |
+--------+----------+-------------+-------+

Header  : 1 Byte
Length  : 2 Bytes (Little Endian)
Payload : Firmware Binary
CRC16   : 2 Bytes (CRC16-CCITT)
```

---

## Bootloader Compatibility

Compatible with:

* 11_HexBoot_F0_IAP

Expected Bootloader Flow:

```text
Receive Header
↓
Receive Payload Length
↓
Receive Firmware Payload
↓
Receive CRC
↓
CRC Verification
↓
Flash Programming
↓
Metadata Generation
↓
Application Validation
↓
Jump To Application
```

---

## Requirements

* Python 3.x

Verify installation:

```bash
python --version
```

or

```bash
py --version
```

---

## Usage

Open a terminal inside the Tools folder:

```bash
py packet_generator.py
```

Enter the firmware binary path when prompted:

```text
..\test_files\11_Hexboot_F0_TEST.bin
```

Example Output:

```text
Packet Created Successfully

Input File     : 11_Hexboot_F0_TEST.bin
Output File    : 11_Hexboot_F0_TEST_packet.bin
Payload Length : 556 bytes
CRC16          : 0xE4D0
Packet Size    : 561 bytes
```

---

## Generated Output

The tool automatically creates:

```text
<firmware_name>_packet.bin
```

Example:

```text
11_Hexboot_F0_TEST.bin

↓

11_Hexboot_F0_TEST_packet.bin
```

---

## CRC Algorithm

CRC16-CCITT

Parameters:

```text
Polynomial : 0x1021
Initial CRC: 0xFFFF
```

This matches the CRC implementation used by the HexBoot_F0 bootloader.

---

## Future Improvements

Potential future enhancements:

* Version field support
* Firmware metadata generation
* Digital signature support
* GUI-based packet generator
* Batch packet generation
* Command-line arguments

---

## Repository

Part of the STM32 Bare-Metal Drivers and HexBoot_F0 learning project.

Author:
Brajo

Year:
2026
