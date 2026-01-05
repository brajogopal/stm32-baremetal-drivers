/*
 * exti.h
 *
 *  Created on: 30-Dec-2025
 *      Author: brajo
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f030x8.h"

#define LINE13 		(1U<<13)

void pc13_exti_init(void);


#endif /* EXTI_H_ */
