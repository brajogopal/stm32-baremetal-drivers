#include "stm32f103x6.h"
//#include <stdio.h>
#include <stdint.h>

#define RCC_APB2ENR_IOPBEN		(1U << 3)
#define RCC_APB2ENR_AFIOEN		(1U << 0)
#define GPIOB_CRL_RESET6		(0xFU << 24)
#define GPIOB_CRL_6_tx			(0xBU << 24)

#define RCC_APB2ENR_uart1EN		(1U << 14)
#define USART1_REMAP			(1U << 2)
#define CR1_TE					(1U << 3)
#define CR1_UE					(1U << 13)

#define SR_TXE					(1 << 7)

void usart1_set_baudrate(uint32_t baudrate);
uint32_t get_pclk2_freq(void);
uint32_t get_sysclk_freq(void);


int main (void){

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	GPIOB->CRL &= ~GPIOB_CRL_RESET6;
	GPIOB->CRL |= GPIOB_CRL_6_tx;

	RCC->APB2ENR |= RCC_APB2ENR_uart1EN;
	AFIO->MAPR |= USART1_REMAP;

    /* 4. USART1 basic config (example 9600 @ 72MHz) */
	//usart1_set_baudrate(9600);     // 9600 baud
	//USART1->BRR |= 0x1D4C;
    USART1->CR1 |= (CR1_TE | CR1_UE);

	while(1){
		while (!(USART1->SR & SR_TXE)){};
		USART1->DR = 'a';
	}
}

uint32_t get_sysclk_freq(void)
{
    uint32_t sws = (RCC->CFGR >> 2) & 0x3;

    switch (sws)
    {
        case 0:  // HSI
            return 8000000U;

        case 1:  // HSE
            return 8000000U;  // typical Blue Pill crystal

        case 2:  // PLL
        {
            uint32_t pllmul = ((RCC->CFGR >> 18) & 0xF) + 2;
            uint32_t pllsrc = (RCC->CFGR >> 16) & 0x1;

            uint32_t pll_input = pllsrc ? 8000000U : 8000000U / 2;
            return pll_input * pllmul;
        }

        default:
            return 8000000U;
    }
}


uint32_t get_pclk2_freq(void)
{
    uint32_t sysclk = get_sysclk_freq();

    uint32_t ahb_presc_table[16] =
    {
        1,1,1,1,1,1,1,1,
        2,4,8,16,64,128,256,512
    };

    uint32_t apb_presc_table[8] =
    {
        1,1,1,1,2,4,8,16
    };

    uint32_t hpre  = (RCC->CFGR >> 4) & 0xF;
    uint32_t ppre2 = (RCC->CFGR >> 11) & 0x7;

    uint32_t hclk  = sysclk / ahb_presc_table[hpre];
    uint32_t pclk2 = hclk   / apb_presc_table[ppre2];

    return pclk2;
}

void usart1_set_baudrate(uint32_t baud)
{
    uint32_t pclk = get_pclk2_freq();

    uint32_t usartdiv_mul100 = (25U * pclk) / (4U * baud);

    uint32_t mantissa = usartdiv_mul100 / 100U;
    uint32_t fraction = (((usartdiv_mul100 - (mantissa * 100U)) * 16U) + 50U) / 100U;

    if (fraction > 15U)
    {
        mantissa += 1U;
        fraction = 0U;
    }

    USART1->BRR = (mantissa << 4) | (fraction & 0xF);
}
