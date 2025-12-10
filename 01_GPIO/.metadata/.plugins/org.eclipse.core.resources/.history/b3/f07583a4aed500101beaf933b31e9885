// LED Connected to PA5 (Port-A & Pin-5)
// Push button to PA6 (Port-A & Pin-6)

#define PERIPH_BASE    (0x40000000UL)

//GPIOA address - 0x4800 0000
#define GPIOA_OFFSET  (0x08000000UL)
#define GPIOA_BASE    (PERIPH_BASE + GPIOA_OFFSET)

//RCC Base address - 0x4002 1000
#define RCC_OFFSET    (0x00021000UL)
#define RCC_BASE      (PERIPH_BASE + RCC_OFFSET)

#define AHBENR_OFFSET (0x14)
#define RCC_AHBENR    (*(volatile unsigned int *)(RCC_BASE + AHBENR_OFFSET)) //TO make it a pointer *(volatile unsigned int *)

#define MODER_OFFSET  (0x00UL)
#define GPIOA_MODER   (*(volatile unsigned int *)(GPIOA_BASE + MODER_OFFSET))

#define ODR_OFFSET    (0x14UL)
#define GPIOA_ODR      (*(volatile unsigned int *)(GPIOA_BASE + ODR_OFFSET))

//need to set Bit-17 to clock enable on GPIOA
#define GPIOAEN       (1U<<17) //0b 0000 0000 0000 0010 0000 0000 0000 0000

#define PIN5          (1U<<5)
#define LED_PIN       PIN5


int main(void){
	/*1.Enable clock access to GPIOA*/
	RCC_AHBENR |= GPIOAEN;

	/* 2.set PA5 as output pin*/
	GPIOA_MODER |= (1U<<10);  //set bit 10 to 1
	GPIOA_MODER &=~(1U<<11);  //set bit 11 to 0

	while(1){
		//set PA5 HIGH
		GPIOA_ODR |= LED_PIN ;
	}
}
