#ifndef I2C_h
#define I2C_h

#include "MKL46Z4.h" 
#include "UART.h"


#define RIGHT_SHIFT(x,y)                   (x >> y)
#define LEFT_SHIFT(x,y)                    (x << y)
#define I2C0_SCL                           (24)
#define I2C0_SDA                           (25)
#define READ(x)                            ((x<<1)|(0x01))
#define WRITE(x)                           ((x<<1)&(0xFE))

 /*  
* Name: I2Cinit      
* Description: Initialization timers etc to right working I2C 
*/ 
void I2C_Init(void);
unsigned char I2C_AddressRead(unsigned char DEV_ADR/*, unsigned char REG_ADR*/);


#endif
