/*
 * common_api.h
 *
 *  Created on: 26-Apr-2026
 *      Author: brajo
 */

#ifndef COMMON_API_H_
#define COMMON_API_H_

#include <stdint.h>
#include "bsp.h"

typedef struct
{
    void (*delay_ms)(uint32_t);
    void (*delay)(uint32_t);
    void (*pinMode)(pin_t, mode_t);
    void (*digitalWrite)(pin_t, uint8_t);
    void (*togglePin)(pin_t);
    uint8_t (*digitalRead)(pin_t);
} btl_common_apis;

#endif /* COMMON_API_H_ */
