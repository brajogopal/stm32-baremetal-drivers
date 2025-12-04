#include <stdio.h>
#include "stm32f030x8.h"
#include <stdint.h>
#include "uart.h"
#include "ADC.h"

uint32_t sensor_value;

int main(void){
	uart1_rxtx_init();
	pa5_adc_init ();


	while(1){
		start_convertion();
		sensor_value = adc_read();
		printf("sensor value : %d \n\r", (int)sensor_value);
	}
}
