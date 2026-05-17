#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include "flash_driver.h"
#include "flash_test.h"
#include <stdio.h>




int main() {
	debug_uart_init(9600);

	println("Starting Flash Erase Test...");

	flash_test_run();

	while (1) {
	}	}

