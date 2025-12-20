# STM32 UART Register-Level Learning Guide

This repository explains a **step-by-step UART learning path** using **STM32 register-level programming**.

It is intended for beginners who want to understand **how UART works internally** before using HAL or other higher-level frameworks.

The focus is on:
- Register-level understanding
- Clock awareness
- Clean driver design
- Gradual progression (TX → modular → RX + TX)

---

## 📁 Folder Structure & Learning Path

Each folder represents **one learning stage**.  
Follow the order strictly if you are a beginner.



01_uart_tx
↓
01_uart_tx2
↓
02_uart_printf
↓
03_uart_modular
↓
04_uart_rx_tx


## 01_uart_tx – Basic UART Transmit

This is the **first UART project**.  
It focuses only on **transmitting data (TX)** from STM32 to a PC terminal.

### What you learn:
- GPIO alternate function configuration
- USART TX enable (TE bit)
- Baud rate calculation using BRR
- TXE flag polling

**Goal:**  
Understand how UART TX works at the **register level**.

---

## 01_uart_tx2 – UART TX with Clock Awareness

This project improves the previous one by introducing **clock awareness**.

Instead of hardcoding clock values, the **APB1 peripheral clock (PCLK1)** is calculated at runtime using RCC registers.

### What you learn:
- Why UART depends on PCLK
- How `get_pclk1_freq()` works
- Why hardcoding clock values is dangerous
- Writing safer UART code

**Goal:**  
Understand why baud-rate issues happen and how to prevent them.

---

## 02_uart_printf – Using `printf()` with UART

In this project, `printf()` is used for the **first time**.

UART output is connected to `printf()` by implementing `__io_putchar()` inside the driver.

### What you learn:
- How `printf()` works internally
- Redirecting standard output to UART
- Difference between character-level TX and formatted output

**Goal:**  
Use UART comfortably for debugging and logging.

---

## 03_uart_modular – Modular UART Driver

This project introduces **proper driver design**.

The code is split into:
- `uart.c` → implementation
- `uart.h` → public API
- `main.c` → application logic

### What you learn:
- Driver vs application separation
- Public API vs private functions
- Why `main.c` should not access registers
- Writing reusable and portable drivers

**Goal:**  
Learn how **real embedded drivers** are written.

---

## 04_uart_rx_tx – Full UART RX & TX

This is the **most complete UART example**.

Both **transmission and reception** are implemented using blocking mode:
- TX on PA2
- RX on PA3

### What you learn:
- RX pin configuration (PA3)
- RXNE flag usage
- Importance of RE bit
- Why RX pin needs pull-up
- Full duplex UART communication

**Goal:**  
Understand complete UART communication (TX + RX).

---

---

## 📌 Final Note

This repository is designed to build a **strong embedded foundation**.

If you understand everything here:
- UART will never feel “magic”
- Baud-rate bugs will make sense
- Driver design will feel natural

Happy learning 🚀
