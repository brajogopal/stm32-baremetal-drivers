/*
 * systick.c
 *
 * SysTick timer driver.
 */

#include "systick.h"
#include "stm32f030x8.h"
#include "uart.h"

#define CTRL_ENABLE     (1U << 0)
#define CTRL_TICKINT    (1U << 1)
#define CTRL_CLKSRC     (1U << 2)


static volatile uint32_t system_tick = 0;

void systick_init(uint32_t tick_hz)
{
    uint32_t clock = get_pclk1_freq();

    SysTick->LOAD = (clock / tick_hz) - 1U;

    SysTick->VAL = 0;

    SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC | CTRL_TICKINT;
}

void SysTick_Handler(void)
{
    system_tick++;
}

uint32_t systick_get_tick(void)
{
    return system_tick;
}

void systick_reset_tick(void)
{
    system_tick = 0;
}
