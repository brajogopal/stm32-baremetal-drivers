#include <stdio.h>
#include <stdint.h>
#include "stm32f030x8.h"
#include "mpu6050.h"


int16_t ax,ay,az,gx,gy,gz;
int16_t ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps;
int16_t temp_raw;
float temp_c;
extern uint8_t mpu_buf[14];




int main(void){

	mpu6050_init();

	while(1){
		mpu6050_read_all();
		ax = (int16_t)((mpu_buf[0] << 8) | mpu_buf[1]);
		ay = (int16_t)((mpu_buf[2] << 8) | mpu_buf[3]);
		az = (int16_t)((mpu_buf[4] << 8) | mpu_buf[5]);

		temp_raw = (int16_t)((mpu_buf[6] << 8) | mpu_buf[7]);

		gx = (int16_t)((mpu_buf[8]  << 8) | mpu_buf[9]);
		gy = (int16_t)((mpu_buf[10] << 8) | mpu_buf[11]);
		gz = (int16_t)((mpu_buf[12] << 8) | mpu_buf[13]);

		ax_g = ax / 16384.0f;
		ay_g = ay / 16384.0f;
		az_g = az / 16384.0f;

		gx_dps = gx / 131.0f;
		gy_dps = gy / 131.0f;
		gz_dps = gz / 131.0f;

		temp_c = (temp_raw / 340.0f) + 36.53f;

	}
}
