#include "shapes.h"
// specifically designed for 128*64 size screens, update as required..

void DrawSquare(uint8_t* ptr,uint8_t x1,uint8_t x2,uint8_t y1,uint8_t y2, SSD1306_COLOR color){

	for(uint8_t x=x1;x<x2;x++){
			ssd1306_DrawPixel( x, y1,  color);
			ssd1306_DrawPixel( x, y2,  color);
	}
	for(uint8_t y=y1;y<y2+1;y++){
			ssd1306_DrawPixel( x1, y,  color);
			ssd1306_DrawPixel( x2, y,  color);
	}
	
}
	
void DrawCircle(uint8_t* ptr,uint8_t x0,uint8_t y0,uint8_t radius, SSD1306_COLOR color) {
		// midpoint circle algorithm
		// https://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C
	
    int16_t f = 1 - radius;
    int16_t ddF_x = 0;
    int16_t ddF_y = -2 * radius;
    uint16_t x = 0;
    uint16_t y = radius;
 
    ssd1306_DrawPixel(x0, y0 + radius,color);
    ssd1306_DrawPixel(x0, y0 - radius,color);
    ssd1306_DrawPixel(x0 + radius, y0,color);
    ssd1306_DrawPixel(x0 - radius, y0,color);
 
    while(x < y) 
    {
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;    
        ssd1306_DrawPixel(x0 + x, y0 + y,color);
        ssd1306_DrawPixel(x0 - x, y0 + y,color);
        ssd1306_DrawPixel(x0 + x, y0 - y,color);
        ssd1306_DrawPixel(x0 - x, y0 - y,color);
        ssd1306_DrawPixel(x0 + y, y0 + x,color);
        ssd1306_DrawPixel(x0 - y, y0 + x,color);
        ssd1306_DrawPixel(x0 + y, y0 - x,color);
        ssd1306_DrawPixel(x0 - y, y0 - x,color);
    }
}

void DrawLine(uint8_t* ptr,uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,SSD1306_COLOR color) {
 
	//https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
	
  int8_t dx = (unsigned)(x1-x0), sx = x0<x1 ? 1 : -1;
  int8_t dy = (unsigned)(y1-y0), sy = y0<y1 ? 1 : -1; 
  int8_t err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    ssd1306_DrawPixel(x0,y0,color);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}
