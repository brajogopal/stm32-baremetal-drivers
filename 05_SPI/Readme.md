SPI & TFT Drivers – STM32 Bare-Metal (Register Level)
====================================================

Overview
--------
This directory contains a progressive set of SPI and TFT driver projects
developed on STM32F030 (Cortex-M0) using pure register-level programming.

- No HAL
- No CubeMX
- No external graphics libraries

The focus is on understanding hardware, registers, and layered driver design.

Each subdirectory represents a stable learning milestone.
Earlier projects are intentionally simple; later ones build cleaner
abstractions on top.


Directory Structure & Learning Flow
-----------------------------------

05_SPI/
├── 01_spi_driver
├── 02_spi_modular_driver
├── 03_tft_spi_print_A
├── 04_tft_text_renderer
└── 05_tft_text_renderer_Advance


Project Breakdown
-----------------

01_spi_driver
-------------
Goal:
Bring up SPI at the lowest possible level.

What is implemented:
- SPI register configuration
- Master mode operation
- Basic data transmit
- GPIO and clock setup

Focus:
Understanding SPI registers and signal flow.


02_spi_modular_driver
---------------------
Goal:
Convert raw SPI code into a reusable driver.

What is implemented:
- spi.c / spi.h separation
- Board-specific SPI control
- Clean and minimal API

Focus:
Driver modularity and code reuse.


03_tft_spi_print_A
------------------
Goal:
Validate SPI + TFT communication.

What is implemented:
- ST7735 TFT initialization
- CASET / RASET window addressing
- RGB565 pixel writes
- Printing a single character ('A')

Focus:
End-to-end SPI → display pipeline.


04_tft_text_renderer
--------------------
Goal:
Move from pixel drawing to text rendering.

What is implemented:
- 5×7 bitmap font
- Character rendering
- String rendering
- Basic text scaling

Focus:
Glyph-to-pixel rendering logic.


05_tft_text_renderer_Advance (Frozen)
-------------------------------------
Goal:
Build a robust and reusable text rendering subsystem.

What is implemented:
- Full printable ASCII support (' ' to '~')
- Cursor-based text output
- Scalable text size
- Foreground and background color control
- Newline ('\n') handling
- Automatic line wrapping
- Word-aware wrapping (space-based)
- Clean, application-friendly API

Design decisions:
- No framebuffer
- No scrolling (intentionally deferred)
- Deterministic memory usage
- Clear separation of driver, renderer, and text layout logic

Status:
- Feature complete
- Frozen for STM32F0
- Scrolling planned later during STM32H7 integration


Design Philosophy
-----------------
- Bare-metal first
- Registers before abstractions
- Freeze stable modules instead of endlessly polishing
- Switch learning layers when novelty drops

This directory focuses on SPI and text fundamentals,
not on full UI frameworks.


What’s Next (Outside This Directory)
-----------------------------------
- Custom bootloader development
- Flash layout and vector table control
- Application / bootloader separation
- Migration to STM32H7

The SPI and TFT work here will serve as a supporting subsystem
in future system-level projects.


Notes
-----
- All subprojects are independent and buildable
- Earlier projects are intentionally not refactored to preserve learning context
- This SPI work is a foundation, not the final goal


Status Summary
--------------
- SPI driver stack: complete
- TFT text rendering: complete (STM32F0)
- Ready to move to bootloader and system-level work

