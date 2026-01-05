#include 	<stdio.h>
#include 	"uart.h"
#include 	"adc.h"
#include "stm32f030x8.h"

#define UART_BAUDRATE	115200
volatile uint32_t sensor_value;

int main (void){

    uart2_tx_init(UART_BAUDRATE);	// Initialize USART2 at 115200 baud
    adc_init(1);
    adc_start_continuous(1);

	while(1){
	    sensor_value = ADC1->DR;
		printf("sensor value : %lu \n\r", sensor_value);
	}
}
