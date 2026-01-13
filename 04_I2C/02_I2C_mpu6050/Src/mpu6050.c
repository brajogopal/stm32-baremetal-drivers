/*
 * mpu6050.c
 *
 *  Created on: 12-Jan-2026
 *      Author: brajo
 */

#include "mpu6050.h"

uint8_t data;
uint8_t mpu_buf[14];


void mpu6050_read_reg (uint8_t reg, uint8_t *data){

	I2C1_byteRead(DEVICE_ADDR, reg, data);

}

void mpu6050_write(uint8_t reg, uint8_t value){

	I2C1_multiByteWrite(DEVICE_ADDR, reg, &value, 1);
}


void mpu6050_read_all(void)
{
    I2C1_multiByteRead(DEVICE_ADDR, 0x3B, mpu_buf, 14);
}



void mpu6050_init(void){
	uint8_t who_am_i;

	/*I2C Init func*/
	I2C1_init();

	/*Read the WHO_AM_I (should return 0x68)*/
	mpu6050_read_reg(WHO_AM_I, &who_am_i);

	/*wake up the device (clear sleep bit)*/
	mpu6050_write(PWR_MGMT_1, 0x00);

    /* 3. Set accelerometer range to ±2g (AFS_SEL = 0) */
    mpu6050_write(ACCEL_CONFIG, 0x00);

}
