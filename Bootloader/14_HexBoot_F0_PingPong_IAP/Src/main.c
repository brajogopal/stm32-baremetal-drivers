#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include "flash_driver.h"
#include "metadata.h"
#include "firmware_receiver.h"
#include "crc.h"
#include <stdio.h>



#define CHUNK_SIZE 128

#define UART_TIMEOUT_LONG 10000000UL


volatile uint8_t rx_buffer[CHUNK_SIZE];
volatile uint8_t chunk_ready = 0;
volatile uint16_t count = 0;
volatile uint16_t rx_index = 0;

volatile uint32_t bytes_received = 0;
volatile uint32_t total = 0;

int main() {
	flash_status_t flash_status;
	uart_status_t uart_status;
	uart_interrupt_init(9600);
	println("Ready to receive Firmware");
		/*
		 Packet Format
		 Byte 0      : Header (FW_HEADER)
		 Byte 1-2    : Payload Length (bytes)
		 Byte 1-2    : CRC16
		 Byte N      : Payload
		 */
	while (1) {
		if(firmware_rx_get_state() == FW_COMPLETE)
		{
		    verify_crc();
		}
	}
}


void USART2_IRQHandler(void)
{
    if(USART2->ISR & UART_RXNE)
    {
        uint8_t data = USART2->RDR;

        firmware_rx_process_byte(data);
    }
}
