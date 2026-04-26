# 🔐 STM32 Bootloader with Shared API (Register-Level)

This repository demonstrates a **step-by-step journey into embedded systems development** using STM32 (bare-metal, register-level programming).

It includes:

* Custom bootloader implementation
* Vector table relocation
* Application jump mechanism
* Shared API between bootloader and application
* Deep understanding of linker scripts and memory layout

---

# 📂 Repository Structure

```text
Bootloader/
│
├── 0_test/                    → Quick experiments & debugging
├── 00_system_driver/          → Core driver development (GPIO, RCC, SysTick)
├── 01_linkerscript_storage/   → Memory mapping & linker script learning
├── 02_bootloader1/            → Basic bootloader implementation
├── 02_bootloader1_app/        → Application for Bootloader1
├── 03_bootloader2/            → Advanced bootloader (Shared API + relocation)
├── 03_bootloader2_app1/       → Application using shared APIs
```

---

# 🧠 Project Breakdown

## 🔹 `0_test`

* Sandbox for quick experiments
* Used for testing register-level behavior

---

## 🔹 `00_system_driver`

* Register-level drivers
* GPIO, RCC, SysTick, basic peripherals
* Foundation layer for all projects

---

## 🔹 `01_linkerscript_storage`

* Flash & RAM memory mapping
* Custom linker scripts
* Understanding section placement

---

## 🔹 `02_bootloader1`

* First bootloader
* MSP setup
* Jump to application

👉 Focus: Boot process fundamentals

---

## 🔹 `02_bootloader1_app`

* Application running from offset flash
* Validates bootloader jump

---

## 🔹 `03_bootloader2`

* Advanced bootloader
* Vector table relocation to SRAM
* Shared API mechanism
* Modular architecture

👉 Focus: Real embedded system design

---

## 🔹 `03_bootloader2_app1`

* Uses shared APIs from bootloader
* Calls functions via function pointers

👉 Focus: Cross-memory execution

---

# 🧠 Memory Layout (Bootloader2)

```text
FLASH (64KB total)

0x08000000 ──────────────── Bootloader (24KB)
0x08006000 ──────────────── Shared API (4KB)
0x08007000 ──────────────── Application
```

0x08000000 ┌────────────────────────────┐
           │       Bootloader           │
           │        (24 KB)             │
0x08006000 ├────────────────────────────┤
           │      Shared API Table      │
           │         (4 KB)             │
0x08007000 ├────────────────────────────┤
           │       Application          │
           │        (Remaining)         │
           └────────────────────────────┘
---

# 🔗 Shared API Mechanism

Bootloader exposes selected functions using a **function pointer table** placed in flash.

### Bootloader:

```c
__attribute__((section(".COMMON_APIS")))
const btl_common_apis common_api_table = {
    .delay_ms = delay_ms,
    .pinMode = pinMode,
    .digitalWrite = digitalWrite,
};
```
### Vector Table Relocation Diagram

Before:
0x00000000 → Bootloader Vector Table

After relocation:
SRAM (0x20000000) ← Application Vector Table
0x00000000 mapped to SRAM

CPU now reads interrupts from SRAM


### Application:

```c
#define COMMON_API_ADDR 0x08006000

const btl_common_apis *common_apis =
    (const btl_common_apis*) COMMON_API_ADDR;

common_apis->pinMode(PA5, OUTPUT);
```
        ┌────────────────────┐
        │   Bootloader       │
        │                    │
        │ common_api_table   │
        │ 0x08006000         │
        │                    │
        └─────────┬──────────┘
                  │ (function pointers)
                  │
                  ▼
        ┌────────────────────┐
        │   Application      │
        │                    │
        │ common_apis->      │
        │    pinMode()       │
        │    digitalWrite()  │
        │                    │
        └────────────────────┘
---

# 🚀 Boot Flow Diagram

[Reset]
   ↓
[Bootloader Start]
   ↓
[Check Application Validity]
   ↓ (invalid)
[Stay in Bootloader]

   ↓ (valid)
[Relocate Vector Table]
   ↓
[Set MSP]
   ↓
[Jump to Application]
   ↓
[Application Running]

---

# ⚠️ Critical Learnings (Real Debugging Experience)

## ❌ 1. "If it works once, it's correct"

Wrong — success can be accidental due to:

* Residual register values
* Clock already enabled
* Memory coincidence

---

## ⚠️ 2. Function pointer ≠ normal function call

```c
common_apis->pinMode();   // Bootloader context
pinMode();                // Application context
```

---

## ⚠️ 3. Static data pitfalls

```c
static GPIO_TypeDef *gpio_ports[];
```

* Exists separately in both images
* Can break when using shared functions

👉 Solution: Use switch-case (safe)

---

## ⚠️ 4. Linker mismatch = silent failure

```c
#define COMMON_API_ADDR 0x08006000
```

Must match:

```ld
CUSTOM_FLASH ORIGIN = 0x08006000
```

---

## 🔍 5. Always verify using `.map` file

```text
.COMMON_APIS  0x08006000
```

👉 This is the **real memory placement**

---

## ⚠️ 6. GPIO clock dependency

Without:

```c
RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
```

GPIO will not work

---

## ⚠️ 7. Vector table vs MSP confusion

* Vector table → interrupt handling
* MSP → stack pointer

---

## ⚠️ 8. Thumb bit requirement

```c
if ((reset & 1) == 0)
```

Missing this → crash

---

## ⚠️ 9. Execution order matters

```c
__disable_irq();
relocate_vector_table();
__set_MSP(...);
jump_to_app();
```

---

## ⚠️ 10. Debugging trap

UART working ≠ system correct

---

# 🧪 Debugging Strategy Used

* Step-by-step UART logs
* Compared direct vs shared calls
* Verified execution flow
* Used `.map` file for validation

---

# 💡 Key Engineering Takeaways

* Linker script defines architecture
* `.map` file shows reality
* Bootloader != normal firmware
* Memory consistency is critical
* Debugging = reasoning, not guessing

---

# 🔮 Future Improvements

* Remove hardcoded addresses using linker symbols
* Add CRC validation
* Add firmware update (UART/USB)
* Multi-stage bootloader
* Transition to STM32H7

---

# 🧑‍💻 Author

Brajo
Embedded Systems Developer | Register-Level Programming

---

# ⭐ Final Note

This repository is not just about building a bootloader —
it’s about understanding **how microcontrollers actually work at a low level**.
 