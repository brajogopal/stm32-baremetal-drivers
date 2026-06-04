/*
 * metadata.c
 *
 *  Created on: 26-May-2026
 *      Author: brajo
 */
#include "metadata.h"
#include <stdlib.h>
#include "uart.h"


storage_data_t data = { .magic_number = STORAGE_MAGIC };
storage_data_t load_data;

void condition_check(void){

	flash_status_t status;
	status = storage_load(&load_data);
	if (status != FLASH_OK) {
		println("flash_load function failed");
		while (1)
			;
	}

	menu();

	char option;
	option = uart_receive_char();
	printf("%c\r\n", option);

	if (option == 'V' || option == 'v') {
		view_metadata(&load_data);
	}

	else if (option == 'U' || option == 'u') {
		update_metadata(&load_data);
	}

	else {
		println("Invalid Option");
	}
}


void view_metadata(storage_data_t *data) {
	printf("magic number = 0x%08lX\r\n", data->magic_number);

	printf("boot count = %lu\r\n", data->boot_count);

	printf("firmware version = %lu\r\n", data->firmware_version);
}

void update_metadata(storage_data_t *data) {
	println("=====================================");
	println(" ");
	println("ENTER MAGIC NUMBER (HEX):");
	println(" ");
	println("=====================================");

	char magic_number_str[20];
	uart_receive_string(magic_number_str, 20);

	uint32_t entered_magic;
	entered_magic = strtoul(magic_number_str, NULL, 16);

	if (entered_magic != data->magic_number) {
		println("INVALID MAGIC NUMBER");
		return;
	} else {
		println("ENTER MAGIC NUMBER MATCHED");
		println(" ");
		println("=====================================");
		println(" ");
		println("ENTER BOOT COUNT :");
		println(" ");
		println("=====================================");
		char boot_count_buff[8];
		uart_receive_string(boot_count_buff, 8);
		uint32_t entered_boot;
		entered_boot = strtoul(boot_count_buff, NULL, 10);
		data->boot_count = entered_boot;

		println("=====================================");
		println(" ");
		println("ENTER FIRMWARE VERSION :");
		println(" ");
		println("=====================================");
		char version_buff[8];
		uart_receive_string(version_buff, 8);
		uint32_t entered_version;
		entered_version = strtoul(version_buff, NULL, 10);
		data->firmware_version = entered_version;
		flash_status_t status = storage_save(data);
		if (status != FLASH_OK) {
			println("flash_save function failed");
			while (1);
		}
		println(" ");
		println("SAVED NEW BOOT COUNT & FIRMWARE VERSION");

		status = storage_load(&load_data);
		if (status != FLASH_OK) {
			println("flash_load function failed");
			while (1);
		}
		println("UPDATED : ");
		view_metadata(&load_data);
		return;
	}
}

void menu(void) {
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
	println("V        ==> VIEW METADATA");
	println("U        ==> UPDATE METADATA");
}
