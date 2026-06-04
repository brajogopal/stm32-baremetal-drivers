/*
 * crc.c
 *
 *  Created on: 01-Jun-2026
 *      Author: brajo
 */

#include "crc.h"
#include "stm32f030x8.h"
#include <stdint.h>
#include <stdio.h>

uint16_t crc16_calculate(uint8_t *data, uint32_t length){

	uint16_t crc = 0xFFFF;

	for(uint32_t i = 0; i < length; i++){

		crc ^= ((uint16_t)data[i] << 8);

		for(int bit=0; bit<8; bit++){
			if(crc & 0x8000){

				crc <<= 1;
				crc ^= CRC16_POLY;
			}
			else	{
				crc <<= 1;
			}
		}
	}
	return crc;
}
