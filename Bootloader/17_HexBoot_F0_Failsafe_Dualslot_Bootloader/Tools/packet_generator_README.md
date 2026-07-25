# Firmware Header Generator

Generate the firmware header required by the Project 17 bootloader.

## Features

- Generate `firmware_header.bin`
- Calculate CRC16 (CCITT)
- Store firmware size
- Support Slot A & Slot B

---

## Input

```
test_application_A.bin
test_application_B.bin
```

---

## Output

```
firmware_header.bin
```

---

## Header Format

```
+--------+----------+-------------+-------------+-----------+-----------+
| Header | Reserved | Slot A Size | Slot B Size | Slot A CRC| Slot B CRC|
+--------+----------+-------------+-------------+-----------+-----------+

Total : 10 Bytes
```

---

## Usage

```bash
py packet_generator.py
```

---

## Project Structure

```
Tools/
├── packet_generator.py
└── README.md
```

---

Part of **Project 17 – HexBoot_F0 Failsafe Dual-Slot IAP**