/*
 * uart.c
 *
 * UART driver.
 */
#include <stdint.h>
#include "uart.h"

#define GPIOAEN		(1U<<17)
#define USART2EN	(1U<<17)
#define CR1_RE		(1U<<2)
#define CR1_TE		(1U<<3)
#define CR1_UE		(1U<<0)
#define ISR_TXE		(1U<<7)


/* Private Functions Prototypes */
static uint16_t compute_uart_brr(uint32_t pclk, uint32_t baudrate);
static void uart_write(char ch);



int __io_putchar(int ch)
{
	uart_write((char)ch);
	return ch;
}


void debug_uart_init(uint32_t baudrate)
{
	/*Enable uart2 in PA2(TX) & PA3(RX)*/
	RCC->AHBENR |= GPIOAEN;

	GPIOA->MODER |= (2U<<4);
	GPIOA->MODER |=	(2U<<6);

	GPIOA->AFR[0]&= ~(0xFU<<8);
	GPIOA->AFR[0]&= ~(0xFU<<12);
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<12);

	RCC->APB1ENR |= USART2EN;
	USART2->BRR = compute_uart_brr(get_pclk1_freq(), baudrate);
	USART2->CR1 |= ( CR1_TE | CR1_RE );
	USART2->CR1 |= CR1_UE;
}


void uart_interrupt_init(uint32_t baudrate)
{
	/*Enable uart2 in PA2(TX) & PA3(RX)*/
	RCC->AHBENR |= GPIOAEN;

	GPIOA->MODER |= (2U<<4);
	GPIOA->MODER |=	(2U<<6);

	GPIOA->AFR[0]&= ~(0xFU<<8);
	GPIOA->AFR[0]&= ~(0xFU<<12);
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<12);

	RCC->APB1ENR |= USART2EN;
	USART2->BRR = compute_uart_brr(get_pclk1_freq(), baudrate);
	USART2->CR1 |= ( CR1_TE | CR1_RE );
	USART2->CR1 |= UART_RXNEIE;
	 __NVIC_EnableIRQ(USART2_IRQn);
	USART2->CR1 |= CR1_UE;
}


static void uart_write(char ch)
{
	while(!(USART2->ISR & ISR_TXE)){}
	USART2->TDR = ch;
}

void println(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }

    for (int i = 0; i < len; i++)
    {
        uart_write(str[i]);
    }

    uart_write('\r');
    uart_write('\n');
}

char uart_receive_char(void)
{
    while((USART2->ISR & UART_RXNE) == 0){}
    return USART2->RDR;
}

uart_status_t uart_receive_with_timeout(uint8_t *data, uint32_t timeout)
{
    while((USART2->ISR & UART_RXNE) == 0){

    	timeout --;
    	if(timeout == 0){
    		return UART_TIMEOUT;
    	}
    }
    *data = USART2->RDR;
    return UART_OK;
}

void uart_receive_string(char *buffer, uint32_t max_length)
{
	char ch;

	for(int i = 0; i <  (max_length-1); i++){

		ch = uart_receive_char();

		/*Enter pressed*/
		if((ch == '\n') || (ch == '\r')){
			buffer[i] = '\0';
			uart_write('\r');
			uart_write('\n');
			return;
		}

		buffer[i] = ch;				//store char in buffer
		uart_write(buffer[i]);		//Echo character
	}
		/* Prevent overflow */
	    buffer[max_length - 1] = '\0';
}


/* Compute USART BRR value (oversampling by 16) */
static uint16_t compute_uart_brr(uint32_t pclk, uint32_t baudrate)
{
    return (pclk + (baudrate / 2U)) / baudrate;
}


/* Get APB1 clock frequency (USART2 clock) */
uint32_t get_pclk1_freq(void)
{
    uint32_t sysclk;
    uint32_t clk_src = (RCC->CFGR >> 2) & 0x3;
    uint32_t hpre    = (RCC->CFGR >> 4) & 0xF;
    uint32_t ppre    = (RCC->CFGR >> 8) & 0x7;
    /* Determine system clock source */
    switch (clk_src)
    {
        case 0x0:  // HSI
            sysclk = 8000000U;
            break;

        case 0x1:  // HSE
            sysclk = 8000000U;   // adjust if crystal differs
            break;

        case 0x2:  // PLL
            sysclk = 48000000U;
            break;

        default:   // reserved / unexpected
            sysclk = 8000000U;   // safe fallback
            break;
    }
    /* Apply AHB prescaler */
    if (hpre >= 8) {			// HPRE: 0xxx=/1, 1000=/2, 1001=/4, 1010=/8 ...
        sysclk >>= (hpre - 7);	// divide SYSCLK by AHB prescaler (2^(hpre-7))
    }
    /* Apply APB1 prescaler */
    if (ppre >= 4) {
        sysclk >>= (ppre - 3); // divide SYSCLK by APB1 prescaler (2^(ppre-3))
    }
    return sysclk;
}
/*Instead of this
 * sysclk = sysclk / 2^1;
 * sysclk = sysclk / 2^2;
 * I used this
 * sysclk >>= 1;
 * sysclk >>= 2;
 * */
