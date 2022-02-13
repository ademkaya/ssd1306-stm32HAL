#ifndef 	_I2CSWH_
#define 	_I2CSWH_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/**
	software i2c library
	Adem KAYA --Feb.2022
*/


#define delayTicks						10U




#define I2C_PORT							GPIOB
#define I2C_PORT_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define SCL_PIN   						GPIO_PIN_6
#define SDA_PIN   						GPIO_PIN_7

#define mDELAY(t)			do{ \
												for (int i = 0; i < t; i++ ) { \
												__NOP(); \
											} \
											}while(0)

											
#define ACK_RECEIVED			false
#define NACK_RECEIVED			true
#define SCL_HIGH()				I2C_PORT->BSRR = SCL_PIN
#define SCL_LOW()					I2C_PORT->BSRR = SCL_PIN<<16U
#define SDA_HIGH()				I2C_PORT->BSRR = SDA_PIN
#define SDA_LOW()					I2C_PORT->BSRR = SDA_PIN<<16U
#define ReadACK(a,b)			HAL_GPIO_ReadPin(a,b)
											
#define START_CONDITION()	do{ \
															SDA_HIGH(); \
															SCL_HIGH(); \
															mDELAY(delayTicks); \
															SDA_LOW(); \
															mDELAY(delayTicks); \
															SCL_LOW(); \
														}while(0)
#define STOP_CONDITION()	do{ \
															SDA_LOW(); \
															SCL_HIGH(); \
															mDELAY(delayTicks); \
															SDA_HIGH(); \
															mDELAY(delayTicks); \
														}while(0)

														
void I2C_SW_Init(void);
bool i2c_sendByte(uint8_t data);														
bool i2c_sendBuffer(uint8_t* byteData,uint32_t size);

														
														
#endif
