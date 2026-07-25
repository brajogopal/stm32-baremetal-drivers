import os
import struct

# ==========================================================
# Configuration
# ==========================================================

FW_HEADER = 0xAA

SLOT_A_FILE = "../test_files/test_application_A.bin"
SLOT_B_FILE = "../test_files/test_application_B.bin"

OUTPUT_HEADER = "../test_files/firmware_header.bin"

CRC16_POLY = 0x1021
CRC16_INIT = 0xFFFF


# ==========================================================
# CRC16 (Matches STM32 crc16_calculate())
# ==========================================================

def crc16_calculate(data: bytes) -> int:
    crc = CRC16_INIT

    for byte in data:
        crc ^= (byte << 8)

        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ CRC16_POLY) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF

    return crc


# ==========================================================
# Firmware Info
# ==========================================================

def firmware_info(path):
    with open(path, "rb") as f:
        data = f.read()

    return {
        "path": path,
        "size": len(data),
        "crc": crc16_calculate(data)
    }


# ==========================================================
# Generate Header
# ==========================================================

slot_a = firmware_info(SLOT_A_FILE)
slot_b = firmware_info(SLOT_B_FILE)

header = struct.pack(
    "<BBHHHH",
    FW_HEADER,          # Header
    0x00,               # Reserved / Padding
    slot_a["size"],
    slot_b["size"],
    slot_a["crc"],
    slot_b["crc"]
)

with open(OUTPUT_HEADER, "wb") as f:
    f.write(header)


# ==========================================================
# Print Summary
# ==========================================================

print("=" * 40)
print("Slot A")
print("=" * 40)
print(f"File : {os.path.abspath(slot_a['path'])}")
print(f"Size : {slot_a['size']} bytes")
print(f"CRC  : 0x{slot_a['crc']:04X}")

print()

print("=" * 40)
print("Slot B")
print("=" * 40)
print(f"File : {os.path.abspath(slot_b['path'])}")
print(f"Size : {slot_b['size']} bytes")
print(f"CRC  : 0x{slot_b['crc']:04X}")

print()

print("=" * 40)
print("Generated")
print("=" * 40)
print("firmware_header.bin")
print(f"Header Size : {len(header)} bytes")

print()

print("Testing Sequence")
print("----------------")
print("1. Send firmware_header.bin")
print("2. Wait for bootloader")
print("3. Bootloader prints REQUEST SLOT")
print("4. Send corresponding firmware image")