/*----------------------------------------------------------------------------
      
 *      Author: Dominik Starzyk
 *---------------------------------------------------------------------------*/

#include "MKL46Z4.h"                    
#include "akcelerometr.h"
#include "I2C.h"
#define who_am_i 0

uint8_t single_data;

void ledInit();

int main (void) {
ledInit();
I2Cinit();
	

	
	
	while(1){	
			single_data = Reading_single_Byte_from_Device();

	}
	
	
	#if (who_am_i ==1)
	if (I2C_ReadRegister_uc (0x3A,0x00, 0x1A)		
	{
	 while(1)
	 {
			ledGreenOn();
	 }
	} 
	else 
	{
	 while(1)
	 {
			ledRedOn();
	 }
	}
	#endif
}

