import os
import struct

HEADER = 0xAA


def crc16_ccitt(data):
    crc = 0xFFFF

    for byte in data:
        crc ^= (byte << 8)

        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF

    return crc


input_file = input("Enter firmware bin file: ")

with open(input_file, "rb") as file:
    firmware_data = file.read()

firmware_length = len(firmware_data)

crc = crc16_ccitt(firmware_data)

packet = bytearray()

# Header
packet.append(HEADER)

# Length (uint16 little-endian)
packet.extend(struct.pack("<H", firmware_length))

# Payload
packet.extend(firmware_data)

# CRC
packet.extend(struct.pack("<H", crc))

output_file = os.path.splitext(input_file)[0] + "_packet.bin"

with open(output_file, "wb") as file:
    file.write(packet)

print("\nPacket Created Successfully")
print("----------------------------")
print(f"Input File     : {input_file}")
print(f"Output File    : {output_file}")
print(f"Payload Length : {firmware_length} bytes")
print(f"CRC16          : 0x{crc:04X}")
print(f"Packet Size    : {len(packet)} bytes")