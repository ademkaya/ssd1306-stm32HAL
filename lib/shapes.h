#ifndef 	_SHAPES_H_
#define 	_SHAPES_H_


#include "ssd1306.h"





void	DrawSquare(uint8_t* ptr,uint8_t x1,uint8_t x2,uint8_t y1,uint8_t y2, SSD1306_COLOR color);
void	DrawCircle(uint8_t* ptr,uint8_t x0,uint8_t y0,uint8_t radius, SSD1306_COLOR color);
void DrawLine(uint8_t* ptr,uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,SSD1306_COLOR color);

















#endif