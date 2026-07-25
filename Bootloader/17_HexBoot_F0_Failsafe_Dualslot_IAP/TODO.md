# TODO

## Project 17

### Current Design

- Boot timeout: 5 seconds
- UART: 19200 baud
- CRC16-CCITT
- Dual-slot firmware update

### Future Improvements (HEXBOOT_F0)

- RCC driver
- Remove UART dependency from Flash driver
- Driver status handlers
- Better layering
- Command-line tools
- Digital signature
- SHA-256

### Repository Cleanup

- [x] Freeze drivers
- [x] Freeze bootloader
- [x] Documentation
- [ ] Release v1.0.0