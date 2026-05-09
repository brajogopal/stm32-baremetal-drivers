## Flash Memory Constraints

The STM32F030C8T6 microcontroller contains 64KB of internal Flash memory organized into fixed erase pages.

Important Flash constraints considered in this bootloader architecture:

- Flash memory cannot be overwritten directly.
- A Flash page must be erased before new data can be programmed.
- Flash erase operations are page-based, not byte-based, STM32F030 Flash erase granularity is 1KB per page.
- Firmware update operations must therefore carefully manage page boundaries and erase sequences.
- While Flash programming/erase operations are ongoing, Flash access latency and execution constraints must be considered.
- The total Flash size (64KB) imposes limitations on firmware slot sizing and upgrade storage capacity.

### Current memory organization:

0x 0800 0000 - 0x 0800 3FFF -> Bootloader (16KB)
0x 0800 4000 - 0x 0800 BFFF -> Main Application Slot (32KB)
0x 0800 C000 - 0x 0800 FFFF -> Upgrade Slot (16KB)


Due to Flash size limitations, the upgrade slot is smaller than the primary application region. This architecture currently supports firmware images that fit within the upgrade slot capacity.

Future improvements planned for higher-end STM32H7 targets:

- Dynamic partition management
- Dual-bank firmware update
- External Flash/QSPI storage
- Advanced rollback mechanisms
- Secure firmware validation

### Bootloader

The bootloader region is programmed during manufacturing or development using an external debug interface such as ST-Link, and cannot be upgraded during application running.This bootloader firmware is located on Flash.
This Firmware also contains UART update logic, Flash write / erase, CRC validation and IAP communication.
This sector allocated only 16kb of flash memory because the bootloader executes only during system startup, firmware update, or recovery operations.

### Main Application 

This region is allocated the largest Flash capacity because the primary application firmware contains the main runtime functionality of the device and executes during normal operation.

### Upgrade Slot 

This is temporary storage area.It store the new upgrade ROM received from IAP communication.


## Current Architectural Limitations

This bootloader architecture is designed primarily as a learning-oriented firmware update system for STM32F0 devices with limited Flash memory resources.

Current limitations include:

- Upgrade slot size is smaller than the maximum application slot size.
- No power-loss-safe update mechanism currently implemented.
- No secure boot or firmware encryption support.
- No rollback recovery mechanism currently implemented.
- Shared persistent boot-state storage not yet implemented.
- Static partition layout used instead of dynamic partition allocation.

## Boot Flow 

RESET
↓
Hardware Init
↓
Read Boot Flags
↓
Validate Main App Header
↓
Validate CRC
↓
Check Update Pending
↓
If valid → Jump
Else → Recovery Mode


### Firmware Update Sequence
1. Bootloader enters update mode.
2. New firmware image is received over UART.
3. Firmware image is stored in Upgrade Slot.
4. CRC validation is performed on received image.
5. Main Application region is erased.
6. Validated image is copied to Main Application Slot.
7. Firmware header and metadata are updated.
8. Boot flags are updated.
9. System jumps to validated application firmware.


## Firmware Header

typedef struct
{
    uint32_t magic_number;
    uint32_t firmware_size;
    uint32_t firmware_crc;
    uint32_t version;
    uint32_t vector_address;
} firmware_header_t;

#### Magic Number
    A known fixed identifier stored in firmware header (eg. #define FW_MAGIC 0xDEADBEEF)

#### CRC
    CRC is stored to verify firmware integrity.

#### Vector Address
    Usually start address of interrupt vector table. Required because bootloader may support - multiple applications, dynamic slots, external flash, relocatable firmware.
    So instead of hardcoding we can use header->vector_address,This makes system more scalable.
    (eg. Instead of "jump_to_app(0x08004000)", now we can do "jump_to_app(header -> vector_address)")