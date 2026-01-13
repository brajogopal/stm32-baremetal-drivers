/*
 * mpu6050.h
 *
 *  Created on: 12-Jan-2026
 *      Author: brajo
 */

#ifndef MPU6050_H_
#define MPU6050_H_
#include "i2c.h"
#include <stdint.h>



#define		WHO_AM_I 		(0x75)
#define		DEVICE_ADDR		(0x68)
#define		PWR_MGMT_1		(0x6B)
#define		PWR_MGMT_2		(0x6C)
#define		ACCEL_CONFIG	(0x1C)
#define		GYRO_CONFIG		(0x1B)
#define		DATA_START_ADDR	(0x3B)
#define 	GYRO_START_ADDR	(0x43)
#define 	TEMP_START_ADDR	(0x41)


void mpu6050_init(void);
void mpu6050_read_all(void);

#endif /* MPU6050_H_ */
