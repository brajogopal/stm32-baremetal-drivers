/*
 * tft.h
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */

#ifndef TFT_H_
#define TFT_H_
#include "stdint.h"


void tft_init(void);
void tft_fill_screen(uint16_t color);
void tft_draw_char(uint16_t x, uint16_t y, char c,uint16_t fg, uint16_t bg, uint8_t size);
void tft_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t fg, uint16_t bg, uint8_t size);

#endif /* TFT_H_ */
