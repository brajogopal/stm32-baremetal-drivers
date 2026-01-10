/*
 * rcc_clock.c
 *
 *  Created on: 08-Jan-2026
 *      Author: brajo
 */
#include "rcc_clock.h"
#include "stm32f030x8.h"


#define CFGR_SWS	(3U<<2)
#define CFGR_HPRE	(0XFU<<4)
#define CFGR_PPRE	(7U<<8)


/*Get RCC_CFGR_SWS*/
uint32_t get_sws(void){
	uint32_t clk_src = ((RCC->CFGR & CFGR_SWS)>>2);
	return(clk_src);
}

uint32_t get_hpre(void){
	uint32_t hclk = ((RCC->CFGR & CFGR_HPRE)>>4);
	return(hclk);
}

uint32_t get_ppre(void){
	uint32_t pclk = ((RCC->CFGR & CFGR_PPRE)>>8);
	return(pclk);
}

uint32_t get_hclk_div(void)
{
	uint32_t hpre_code = get_hpre();

    if (hpre_code < 8)
    {
    return 1;   // SYSCLK not divided
    }
    else if (hpre_code == 8)
    {
        return 2;
    }
    else if (hpre_code == 9)
    {
        return 4;
    }
        else if (hpre_code == 10)
    {
    	return 8;
    }
        else if (hpre_code == 11)
    {
    	return 16;
    }
        else if (hpre_code == 12)
   	{
    	return 64;
    }
    	else if (hpre_code == 13)
    {
    	return 128;
    }
    	else if (hpre_code == 14)
    {
    	 return 256;
    }
    	 else // 15
    {
    	 return 512;
    }
}


uint32_t get_pclk_div(void)
{
	uint32_t ppre_code = get_ppre();

    if (ppre_code < 4)
    {
    	return 1;   // HCLK not divided
    }
    else if (ppre_code == 4)
    {
    	return 2;
    }
    else if (ppre_code == 5)
    {
        return 4;
    }
    else if (ppre_code == 6)
    {
        return 8;
    }
    else
    {
        return 16;
    }
}


uint32_t rcc_get_pclk1_freq(void)
{
    uint32_t sysclk;
    uint32_t hclk;
    uint32_t pclk1;

    /* 1. Determine SYSCLK source */
    switch (get_sws())
    {
        case 0: /* HSI */
            sysclk = 8000000U;
            break;

        case 1: /* HSE */
            sysclk = 8000000U;   // change if external crystal differs
            break;

        case 2: /* PLL */
            sysclk = 48000000U;
            break;

        default:
            sysclk = 8000000U;   // safe fallback
            break;
    }

    /* 2. Apply AHB prescaler */
    hclk = sysclk / get_hclk_div();

    /* 3. Apply APB1 prescaler */
    pclk1 = hclk / get_pclk_div();

    return pclk1;
}




