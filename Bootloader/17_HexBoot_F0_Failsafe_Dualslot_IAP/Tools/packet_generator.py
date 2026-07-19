#!/usr/bin/env python3

import argparse
import struct
from pathlib import Path

# ----------------------------------------------------------
# Configuration
# ----------------------------------------------------------

FW_HEADER = 0xAA


# ----------------------------------------------------------
# CRC16 (Modbus)
# ----------------------------------------------------------

def crc16(data: bytes) -> int:

    crc = 0xFFFF

    for byte in data:

        crc ^= byte

        for _ in range(8):

            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1

    return crc & 0xFFFF


# ----------------------------------------------------------
# Read Firmware
# ----------------------------------------------------------

def firmware_info(filename):

    with open(filename, "rb") as f:
        data = f.read()

    return {
        "size": len(data),
        "crc": crc16(data)
    }


# ----------------------------------------------------------
# Main
# ----------------------------------------------------------

def main():

    parser = argparse.ArgumentParser(
        description="HEXBOOT_F0 Header Generator"
    )

    parser.add_argument(
        "slot_a",
        help="Firmware image for Slot A"
    )

    parser.add_argument(
        "slot_b",
        help="Firmware image for Slot B"
    )

    parser.add_argument(
        "-o",
        "--output",
        default="firmware_header.bin",
        help="Output header filename"
    )

    args = parser.parse_args()

    slot_a = firmware_info(args.slot_a)
    slot_b = firmware_info(args.slot_b)

    print()
    print("----------------------------------------")
    print(" Slot A")
    print("----------------------------------------")
    print(f"File : {args.slot_a}")
    print(f"Size : {slot_a['size']} bytes")
    print(f"CRC  : 0x{slot_a['crc']:04X}")

    print()
    print("----------------------------------------")
    print(" Slot B")
    print("----------------------------------------")
    print(f"File : {args.slot_b}")
    print(f"Size : {slot_b['size']} bytes")
    print(f"CRC  : 0x{slot_b['crc']:04X}")

    #
    # Header Layout
    #
    # Byte 0  : Header
    # Byte 1  : Reserved (Padding)
    # Byte 2-3: Payload Length A
    # Byte 4-5: Payload Length B
    # Byte 6-7: CRC A
    # Byte 8-9: CRC B
    #

    header = struct.pack(
        "<BBHHHH",
        FW_HEADER,
        0x00,                  # Reserved / Padding
        slot_a["size"],
        slot_b["size"],
        slot_a["crc"],
        slot_b["crc"]
    )

    with open(args.output, "wb") as f:
        f.write(header)

    print()
    print("----------------------------------------")
    print("Generated")
    print("----------------------------------------")
    print(args.output)
    print(f"Header Size : {len(header)} bytes")
    print()

    print("Testing Sequence")
    print("----------------")
    print("1. Send firmware_header.bin")
    print("2. Wait for bootloader")
    print("3. Bootloader prints REQUEST SLOT")
    print("4. Send corresponding firmware image")


if __name__ == "__main__":
    main()