#include "i2c_sw.h"


static inline void I2C__SetInput(void);
static inline void I2C__SetOutput(void);


void I2C_SW_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  I2C_PORT_CLK_ENABLE();

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = SCL_PIN|SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;  
  HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET); 
}

static inline void I2C__SetInput(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
	I2C_PORT_CLK_ENABLE();

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}

static inline void I2C__SetOutput(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
	I2C_PORT_CLK_ENABLE();

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}







bool i2c_sendBuffer(uint8_t* byteData,uint32_t size){
	bool nack_received=false;
		for(uint32_t x =0;x<size;x++)  {
			if (i2c_sendByte(byteData[x]) == NACK_RECEIVED){
				nack_received = true;
				break;
			}
		}
		return nack_received;
}

bool i2c_sendByte(uint8_t data)		{
	
			for (uint8_t y =0;y<8;y++)	{
					if((data>>(7-y))&0x01)	{
							SDA_HIGH();
					}else{
							SDA_LOW();
					}
					mDELAY(delayTicks); 
				SCL_HIGH();
					mDELAY(delayTicks); 	
				SCL_LOW();
			}
			
			SDA_HIGH();
			mDELAY(delayTicks); 
			
			SCL_HIGH();
					mDELAY(delayTicks);
			// ----- read back the ack data -----
				I2C__SetInput();
					bool retVal = ReadACK(I2C_PORT,SDA_PIN);
				I2C__SetOutput();
			// ----------------------------------
			SCL_LOW();
					mDELAY(delayTicks);			

			return retVal;
}



