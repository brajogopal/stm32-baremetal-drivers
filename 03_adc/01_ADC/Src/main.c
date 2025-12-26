#include 	<stdio.h>
#include 	"uart.h"
#include 	"adc.h"

#define UART_BAUDRATE	115200

int main (void){

    uart2_tx_init(UART_BAUDRATE);	// Initialize USART2 at 115200 baud
    adc_init(1);

	while(1){
		uint32_t sensor_value = adc_read(1);
		printf("sensor value : %d \n\r", sensor_value);
	}
}
