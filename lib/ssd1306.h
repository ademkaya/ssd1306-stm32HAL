/**
 * This Library is written and optimized by Olivier Van den Eede(4ilo) in 2016
 * for Stm32 Uc and HAL-i2c lib's.
 *
 * To use this library with ssd1306 oled display you will need to customize the defines below.
 *
 * This library uses 2 extra files (fonts.c/h).
 * In this files are 3 different fonts you can use:
 *     - Font_7x10
 *     - Font_11x18
 *     - Font_16x26
 *
 *
 *	@Revisions : Software I2C is added Feb.2022
 */

#ifndef _SSD1306_H
#define _SSD1306_H

#include "stm32f4xx_hal.h"
#include "i2c_sw.h"
#include "fonts.h"


/**
	@brief : if the I2C handler is set to NULL, then it initializes the software I2C
*/

// I2c address
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR        0x78
#endif // SSD1306_I2C_ADDR

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           128
#endif // SSD1306_WIDTH

// SSD1306 LCD height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          64
#endif // SSD1306_HEIGHT





// commands
#define		WriteCOMMAND	(uint8_t)0x00
#define		WriteDATA			(uint8_t)0x40

#define 	SET_CONTRAST							(uint8_t)0x81			// requires arg between [0 -  0xFF]

#define		ALLOW_RAM_CONTENT					(uint8_t)0xA4
#define		IGNORE_RAM_CONTENT				(uint8_t)0xA5

#define		NORMAL_DISPLAY						(uint8_t)0xA6
#define		INVERT_DISPLAY						(uint8_t)0xA7

#define		SET_DISPLAY_ON						(uint8_t)0xAF
#define		SET_DISPLAY_OFF						(uint8_t)0xAE

//3. Addressing Setting Command Table 
#define 	Addressing_MODE							(uint8_t)0x20		//  requires inputs
		#define		Vertical_ADDRESSING			(uint8_t)0x01
		#define		Horizontal_ADDRESSING		(uint8_t)0x00	
		#define		Page_ADDRESSING					(uint8_t)0x02 	//  selected after reset as default

#define		SETUP_COLUMN_ADDRESS				(uint8_t)0x21 	//  requires inputs
		// column_start_ADDRESS			 			0 - 127
		// column_end_ADDRESS			 	 			0 - 127

#define 	SETUP_PAGE_ADDRESS					(uint8_t)0x22		//	requires inputs
		// page_start_address							0 - 127
		// page_end_address								0 - 127


#define 	SET_Display_Start_Line			(uint8_t)0x40
#define		SET_COM_Scan_Direction			(uint8_t)0xC8 // C0 - C8
#define		SET_Segment_Remap						(uint8_t)0xA1 // A0 - A1
#define		SET_MUX_Ratio								(uint8_t)0xA8
#define		SET_Display_Offset					(uint8_t)0xA8
#define		SET_Clock_Ratio							(uint8_t)0xD5	//	requires input
		// 	A[3:0] > clock ratio
		//  A[7:4] > osc. freq.							
#define		SET_Precharge_Period				(uint8_t)0xD9	//  requires input	
#define		Charge_Pump_MODE						(uint8_t)0x8D	//  requires input	
	#define CP_ON												(uint8_t)0x14	
	#define CP_OFF											(uint8_t)0x10	





//
//  Enumeration for screen colors
//
typedef enum {
    Black = 0x00,   // Black color, no pixel
    White = 0x01,   // Pixel is set. Color depends on LCD
} SSD1306_COLOR;

//
//  Struct to store transformations
//
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

//
//  Function definitions
//

uint8_t ssd1306_Init(I2C_HandleTypeDef *hi2c);
bool ssd1306_UpdateScreen(I2C_HandleTypeDef *hi2c);

void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_InvertColors(void);

#endif  // _SSD1306_H
