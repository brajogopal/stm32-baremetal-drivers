/*
 * bsp.h
 *
 *  Created on: 01-Mar-2026
 *      Author: BROJOGOPAL
 */
#ifndef BSP_H_
#define BSP_H_

#include <stdint.h>

/* Pin type */

typedef uint8_t pin_t;

/* Pin modes */

typedef enum
{
    INPUT   = 0x0,
    OUTPUT  = 0x1,
    AF      = 0x2,
    ANALOG  = 0x3

} mode_t;

/* Logic states */

#define HIGH 1
#define LOW  0

/* Function prototypes */

void pinMode(pin_t pin, mode_t mode);
void digitalWrite(pin_t pin, uint8_t state);
uint8_t digitalRead(pin_t pin);

/* Pin definitions */

#define PA0  (0x00)
#define PA1  (0x01)
#define PA2  (0x02)
#define PA3  (0x03)
#define PA4  (0x04)
#define PA5  (0x05)
#define PA6  (0x06)
#define PA7  (0x07)
#define PA8  (0x08)
#define PA9  (0x09)
#define PA10 (0x0A)
#define PA11 (0x0B)
#define PA12 (0x0C)
#define PA13 (0x0D)
#define PA14 (0x0E)
#define PA15 (0x0F)

#define PB0  (0x10)
#define PB1  (0x11)
#define PB2  (0x12)
#define PB3  (0x13)
#define PB4  (0x14)
#define PB5  (0x15)
#define PB6  (0x16)
#define PB7  (0x17)
#define PB8  (0x18)
#define PB9  (0x19)

#define PC0  (0x20)
#define PC1  (0x21)

/* Pin decoding */

#define PIN_PORT(pin)   ((pin) >> 4)
#define PIN_NUMBER(pin) ((pin) & 0x0F)

#endif
