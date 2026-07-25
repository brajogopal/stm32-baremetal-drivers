/*
 * crc.h
 *
 * Public interface for CRC-16 calculation.
 */

#ifndef CRC_H_
#define CRC_H_

#include <stdint.h>

#define CRC16_POLY	0x1021U	//Polynomial: x^16 + x^12 + x^5 + 1

uint16_t crc16_calculate(const uint8_t *data, uint32_t length);	// Currently received as uint16_t, but stored as uint32_t for future expansion

#endif /* CRC_H_ */
