/*
 * i2c.h
 *
 *  Created on: 07-Jan-2026
 *      Author: brajo
 */

#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>

void I2C1_init (void);
void I2C1_byteRead(uint8_t saddr, uint8_t maddr, uint8_t *data);
void I2C1_multiByteRead(uint8_t saddr, uint8_t maddr, uint8_t *data, uint8_t len);
void I2C1_multiByteWrite(uint8_t saddr, uint8_t maddr, uint8_t *data, uint8_t len);

#endif /* I2C_H_ */
