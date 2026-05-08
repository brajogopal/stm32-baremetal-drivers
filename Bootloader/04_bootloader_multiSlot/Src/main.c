#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include <stdio.h>

#define SECTOR0_BASE_ADDRESS	0x08000000	//bootloader sector
#define SECTOR1_BASE_ADDRESS	0x08004000	//Default APP sector
#define SECTOR2_BASE_ADDRESS	0x08008000	//APP1 sector
#define SECTOR3_BASE_ADDRESS	0x0800C000	//FACTORY APP SECTOR

#define DEFAULT_APP_ADDRESS		SECTOR1_BASE_ADDRESS
#define APP1_ADDRESS			SECTOR2_BASE_ADDRESS
#define FACTORY_APP_ADDRESS		SECTOR3_BASE_ADDRESS

volatile char g_ch_key;
volatile uint8_t g_un_key;


typedef enum{
	APP1 = 1,
	FACTORY_APP,
}SYS_APPS;

static void process_btldr_cmds(SYS_APPS curr_app);

/*
static void process_btldr_cmds(SYS_APPS curr_app);
__attribute__((section(".COMMON_APIS"))) const btl_common_apis common_api_table = {
		.delay_ms = delay_ms,
		.delay = delay,
		.pinMode = pinMode,
		.digitalWrite = digitalWrite,
		.togglePin = togglePin,
		.digitalRead = digitalRead
};
*/
int main() {
	system_uart_init(9600);
	pinMode(PA6, INPUT);
	if(digitalRead(PA6)){
		println("button pressed");
		println("=====================================");
		println("=====================================");
		println("=====================================");
		println(" ");
		println("Bootloader Menu");
		println(" ");
		println("=====================================");
		println("=====================================");
		println("=====================================");
		println("Avilable Command: ");
		println("1        ==> RUN APP");
		println("F        ==> FACTORY APP");

		while(1){
			process_btldr_cmds(g_un_key);
		}
	}
	else{
		//button is not pressed
		jmp_to_app(DEFAULT_APP_ADDRESS);
	}
	while (1) {
	}
}

static void process_btldr_cmds(SYS_APPS curr_app){

	switch(curr_app){
	case APP1:
		println("APP1 SELECTED");
		jmp_to_app(APP1_ADDRESS);	//do something
		break;
	case FACTORY_APP:
		println("FACTORY APP SELECTED");
		jmp_to_app(FACTORY_APP_ADDRESS);	//do something
		break;
	default:

	}
}



static void usart_callback(void){
	g_ch_key = USART2->RDR;
	if(g_ch_key == '1'){
		println("Pressed Key : 1");
		g_un_key = 1;
	}
	else if((g_ch_key == 'F') || (g_ch_key == 'f')){
		g_un_key = 2;
	}
	else	{
		println("Pressed Key : other");
	}
}

void USART2_IRQHandler(void){

	if(USART2->ISR & SR_RXNE)	//check if RXNE is set
	{
		usart_callback();
	}
}
