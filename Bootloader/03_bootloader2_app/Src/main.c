

#include "stm32f030x8.h"
#include "bsp.h"
#include <stdio.h>

#define COMMON_API_ADDR		0x08002000

typedef struct
{
    void (*delay_ms)(uint32_t tim);
    void (*delay)(uint32_t sec);
    void (*pinMode)(pin_t pin, mode_t mode);
    void (*digitalWrite)(pin_t pin, uint8_t state);
} btl_common_apis;


int main(void){
	const btl_common_apis *common_apis = (const btl_common_apis*) COMMON_API_ADDR;

	RCC->AHBENR |= (1U<<17);

	GPIOA->MODER &= ~(3U << 10);
	GPIOA->MODER |= (1U << 10);
	while(1){
		common_apis->digitalWrite(PA5, HIGH);
		common_apis->delay_ms(500);

		common_apis->digitalWrite(PA5, LOW);
		common_apis->delay_ms(500);
	}
}
