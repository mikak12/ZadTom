/*----------------------------------------------------------------------------
      
 *      Author: Dominik Starzyk
 *---------------------------------------------------------------------------*/

#include "MKL46Z4.h"                    
#include "akcelerometr.h"
#include "I2C.h"
#include "Led.h"
#include "UART.h"

unsigned char single_byte;

int main (void) 
{
	uint32_t ii = 0 ;
	unsigned char data_Tab[]={84, 79, 77, 69, 75};
	uint8_t devAddr = 0;
	uint8_t addrCnt = 0;
	LED_Init();
	I2C_Init();
	uart0Init();

//	for(devAddr = 0; devAddr < 128; devAddr++)
//	{
//		if(I2C_AddressRead(devAddr) == 0)
//		{
//			addrCnt++;
//		}
//	}
	
		UART0_Transmit(32);  
	
			for(ii=0; ii<5; ii++)
	{
		UART0_Transmit(data_Tab[ii]);
	}
	UART0_Transmit(32);
	
	/*I2C_AddressRead(0x1D);
	
	
	
	I2C_AddressRead(0x1D);
	
	for(ii = 0; ii < 999999; ii++)
	{}

	I2C_AddressRead(0x1D);
	I2C_AddressRead(0x1D);
		
	for(ii = 0; ii < 999999; ii++)
	{}
		
	I2C_AddressRead(0x0E);
	I2C_AddressRead(0x0E);
	
	for(ii = 0; ii < 999999; ii++)
	{}

	I2C_AddressRead(0x0E);
	I2C_AddressRead(0x0E);*/
	
	//	Status_ACK_NACK();
		
	while(1)
	{	
//		single_byte = I2C_ReadSingle_Byte(0x1D, 0x0D);
//		UART0_Transmit(single_byte);
	}
}

