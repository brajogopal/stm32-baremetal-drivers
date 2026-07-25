/*
 * systick.h
 *
 * Public interface for SysTick timer.
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

void systick_init(uint32_t tick_hz);

uint32_t systick_get_tick(void);

void systick_reset_tick(void);

#endif /* SYSTICK_H_ */
