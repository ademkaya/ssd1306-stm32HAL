#include "ssd1306.h"

uint8_t initData[] = { 	SET_DISPLAY_OFF, \
												Addressing_MODE, Horizontal_ADDRESSING, \
												SET_Display_Start_Line, \
												SET_CONTRAST,0xFF, \
												NORMAL_DISPLAY, \
												SET_MUX_Ratio,SSD1306_HEIGHT - 1, \
												SET_COM_Scan_Direction, \
												SET_Segment_Remap, \
												Charge_Pump_MODE,CP_ON, \
												ALLOW_RAM_CONTENT,SET_DISPLAY_ON};


// Screenbuffer
uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Screen object
static SSD1306_t SSD1306;


//
//  Send a byte to the command register
//
static uint8_t ssd1306_WriteCommand(I2C_HandleTypeDef *hi2c, uint8_t command)
{
	uint8_t answer = ACK_RECEIVED;
		if (hi2c != NULL)
				answer =  HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR, WriteCOMMAND, 1, &command, 1, 10);
		else{
			START_CONDITION();	
				if(i2c_sendByte(SSD1306_I2C_ADDR) == NACK_RECEIVED)
					{answer  = NACK_RECEIVED;return answer;}
				if(i2c_sendByte(WriteCOMMAND) == NACK_RECEIVED)
					{answer  = NACK_RECEIVED;return answer;}
				if(i2c_sendByte(command) == NACK_RECEIVED)
					{answer  = NACK_RECEIVED;return answer;}
			STOP_CONDITION();
										
		}
		return answer;
}


//
//  Initialize the oled screen
//
uint8_t ssd1306_Init(I2C_HandleTypeDef *hi2c)
{
    // Wait for the screen to boot
    HAL_Delay(100);
    bool status = 0;

		if(hi2c == NULL){
			I2C_SW_Init();		
		}
	
    // Init LCD
		for(uint16_t i = 0;i<sizeof(initData);i++){
			if (ssd1306_WriteCommand(hi2c,initData[i])== NACK_RECEIVED){
				status = NACK_RECEIVED;
				break;
			}
		}

    if (status != ACK_RECEIVED) {
        return NACK_RECEIVED;
    }

    // Clear screen
    ssd1306_Fill(Black);

    // Flush buffer to screen
    ssd1306_UpdateScreen(hi2c);

    // Set default values for screen object
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;

    SSD1306.Initialized = 1;

    return 0;
}

//
//  Fill the whole screen with the given color
//
void ssd1306_Fill(SSD1306_COLOR color)
{
    // Fill screenbuffer with a constant value (color)
    uint32_t i;

    for(i = 0; i < sizeof(SSD1306_Buffer); i++)
    {
        SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

uint8_t* staticArrayPtr(void){
	return &SSD1306_Buffer[0];
}

//
//  Write the screenbuffer with changed to the screen
//
bool ssd1306_UpdateScreen(I2C_HandleTypeDef *hi2c)
{
    bool status = ACK_RECEIVED;
		
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,SETUP_COLUMN_ADDRESS) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}				
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,0) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,SSD1306_WIDTH-1) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}
		
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,SETUP_PAGE_ADDRESS) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}				
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,0) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,(SSD1306_HEIGHT/8)-1) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}
		
		if (hi2c != NULL)
			status = HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR, WriteDATA, 1, &SSD1306_Buffer[0], sizeof(SSD1306_Buffer), 100);			
		else{
			START_CONDITION();
				if((status==ACK_RECEIVED)&&(i2c_sendByte(SSD1306_I2C_ADDR)== NACK_RECEIVED))
				{status  = NACK_RECEIVED;return status;}
				if((status==ACK_RECEIVED)&&(i2c_sendByte(WriteDATA)== NACK_RECEIVED))
				{status  = NACK_RECEIVED;return status;}
				if((status==ACK_RECEIVED)&&(i2c_sendBuffer(&SSD1306_Buffer[0],sizeof(SSD1306_Buffer))== NACK_RECEIVED))
				{status  = NACK_RECEIVED;return status;}
			STOP_CONDITION();			
		}
		
		return status;
}
/**
	x1,x2 >  0 to  SSD1306_WIDTH 		- 1
	y1-y2 >  0 to  SSD1306_HEIGTH/8 - 1
*/
bool ssd1306_UpdateRegion(I2C_HandleTypeDef *hi2c,uint8_t x1,uint8_t x2,uint8_t page_y1,uint8_t page_y2)
{
    bool status = ACK_RECEIVED;

		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,SETUP_COLUMN_ADDRESS) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}				
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,x1) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,x2) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}
		
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,SETUP_PAGE_ADDRESS) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}				
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,page_y1) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}
		if((status==ACK_RECEIVED)&&(ssd1306_WriteCommand(hi2c,page_y2) == NACK_RECEIVED))
		{status  = NACK_RECEIVED;return status;}
		
		uint16_t start=0;
		uint16_t size = 0;
		if (hi2c != NULL)
				for (uint8_t i = page_y1;i<page_y2+1;i++){	
							start = SSD1306_WIDTH*i + x1;
							size  = x2-x1+1;
							status = HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR, WriteDATA, 1, &SSD1306_Buffer[start], size, 100);
				}
		else{
			START_CONDITION();
				if((status==ACK_RECEIVED)&&(i2c_sendByte(SSD1306_I2C_ADDR)== NACK_RECEIVED))
				{status  = NACK_RECEIVED;return status;}
				if((status==ACK_RECEIVED)&&(i2c_sendByte(WriteDATA)== NACK_RECEIVED))
				{status  = NACK_RECEIVED;return status;}
				if(status==ACK_RECEIVED)
				{
					for (uint8_t i = page_y1;i<page_y2+1;i++){	
							start = SSD1306_WIDTH*i + x1;
							size  = x2-x1+1;
							if (i2c_sendBuffer(&SSD1306_Buffer[start],size)== NACK_RECEIVED)
							{status  = NACK_RECEIVED;return status;}
					}
				}				
			STOP_CONDITION();			
		}
		
		return status;
}




//
//  Draw one pixel in the screenbuffer
//  X => X Coordinate
//  Y => Y Coordinate
//  color => Pixel color
//
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        // Don't write outside the buffer
        return;
    }

    // Check if pixel should be inverted
    if (SSD1306.Inverted)
    {
        color = (SSD1306_COLOR)!color;
    }

    // Draw in the correct color
    if (color == White)
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}


//
//  Draw 1 char to the screen buffer
//  ch      => Character to write
//  Font    => Font to use
//  color   => Black or White
//
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
    uint32_t i, b, j;

    // Check remaining space on current line
    if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
        SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
    {
        // Not enough space on current line
        return 0;
    }

    // Translate font to screenbuffer
    for (i = 0; i < Font.FontHeight; i++)
    {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for (j = 0; j < Font.FontWidth; j++)
        {
            if ((b << j) & 0x8000)
            {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            }
            else
            {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }

    // The current space is now taken
    SSD1306.CurrentX += Font.FontWidth;

    // Return written char for validation
    return ch;
}

//
//  Write full string to screenbuffer
//
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color)
{
    // Write until null-byte
    while (*str)
    {
        if (ssd1306_WriteChar(*str, Font, color) != *str)
        {
            // Char could not be written
            return *str;
        }

        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

//
//  Invert background/foreground colors
//
void ssd1306_InvertColors(void)
{
    SSD1306.Inverted = !SSD1306.Inverted;
}

//
//  Set cursor position
//
void ssd1306_SetCursor(uint8_t x, uint8_t y)
{
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}
