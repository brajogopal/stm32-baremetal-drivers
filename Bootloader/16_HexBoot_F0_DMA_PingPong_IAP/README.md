# 15_HexBoot_F0_PingPong_Buffer_IAP

## Overview

This project extends the interrupt-based firmware update system developed in Project 14 by introducing a Ping-Pong Buffer architecture.

The objective is to decouple firmware reception from flash programming by maintaining separate receive and programming buffers.

Firmware data is received through UART interrupts, stored in the active receive buffer, and programmed to flash using the inactive programming buffer after a buffer swap.

---

## Features

- UART Interrupt Based Firmware Reception
- Packet Based Firmware Protocol
- Firmware Header Validation
- Firmware Length Validation
- CRC Validation
- Ping-Pong Buffer Architecture
- Chunk Based Flash Programming
- Application Metadata Support
- Firmware Integrity Verification

---

## Ping-Pong Buffer Architecture

```text
UART Interrupt
      |
      v
Firmware Packet Parser
      |
      v
Ping-Pong Buffer
      |
      +--> Receive Buffer
      |
      +--> Programming Buffer
      |
      v
Flash Programming
      |
      v
CRC Verification
```

---

## Project Structure

```text
Inc/
├── firmware_receiver.h
├── firmware_pingpong.h
├── flash_driver.h
├── crc.h
├── metadata.h

Src/
├── firmware_receiver.c
├── firmware_pingpong.c
├── flash_driver.c
├── crc.c
├── metadata.c
├── main.c
```

---

## Key Functions

### firmware_pingpong_init()

Initializes Ping-Pong buffers and internal state.

### firmware_pingpong_process_byte()

Stores incoming UART data into the active receive buffer.

### swap_buffers()

Swaps receive and programming buffers when a chunk becomes ready.

---

## Test Result

Firmware Size:

```text
556 Bytes
```

Result:

```text
Header matched
Length: 556
CRC: 0xE4D0

FLASH_OK
FLASH_OK
FLASH_OK
FLASH_OK
FLASH_OK

Calculated CRC : 0xE4D0
CRC verified successfully
```

---

## Performance Investigation

| Baud Rate | Minimum Stable Delay |
|------------|---------------------|
| 9600 | 20 ms |
| 19200 | 15 ms |
| 38400 | Unstable at low delays |

---

## Debug Investigation

During throughput testing, USART Overrun Error (ORE) was observed at higher transfer rates.

Observed behavior:

```text
ORE : 1
```

Investigation performed:

- Flash programming temporarily removed
- Debug prints removed
- ORE still occurred

Conclusion:

The primary bottleneck is interrupt-per-byte UART reception.

---

## Limitations

- UART reception uses one interrupt per received byte.
- ORE may occur at higher transfer rates.
- No automatic transfer recovery after ORE.
- Firmware update remains incomplete when payload bytes are lost.

---

## Future Improvements

Project 16 will introduce:

- UART DMA Reception
- Reduced interrupt overhead
- Improved transfer throughput
- Better scalability for larger firmware images

---

## Screenshots

### Successful Firmware Transfer

`01_PingPong_IAP_Successful_Firmware_Transfer.png`

### UART Overrun Investigation

`02_UART_Overrun_Error_Investigation.png`

---

## Target MCU

STM32F030C8T6

ARM Cortex-M0

---

## Author

Brajo Gopal Chakraborty