#ifndef I2C_h
#define I2C_h

#include "MKL46Z4.h"  

void I2C_EnableACK(void);
void I2C_DisableACK_NACK(void);
void I2C_Start(void);
void I2C_TRAN(void);
void I2C_REC(void);
void I2C_RepeatedStart(void);
void I2C_Stop(void);

void Busy_ACK(void);
void Tran_Comp(void);
void I2C_wait(void);
void NACK(void);
void ACK(void);

void I2C_EnterRxMode(void);
void I2C_write_byte(uint32_t data);
char I2C_read_byte(void);


 /*  
* Name: I2Cinit      
* Description: Initialization timers etc to right working I2C 
*/ 
void I2Cinit(void);
 /*  
* Name: READING....  
* Requires: Device Address, Device Register address  
* Returns: in single byte return 8bit data from device 
* Description: configuration connection between slave and master  
*/ 
uint8_t Reading_single_Byte_from_Device(void);
void Reading_Multiple_Bytes_set_up(void);   //MRSW
void Master_read_Slave_write_SET_up(void);   //moj pomysl prawie taki jak wyzej

/*--------------------------------*/
 /*  
* Name: I2C_ReadRegister_uc  
* Requires: Device Address, Device Register address  
* Returns: unsigned char 8 bit data received from device  
* Description: Reads 8 bits of data from device register and returns it  
*/ 
unsigned char I2C_ReadRegister_uc (unsigned char u8Address, unsigned char u8Register );

/*  
* Name: I2C_ReadRegister  
* Requires: Device Address, Device Register address, Pointer for returned data    
* Description: Reads device register and puts it in pointer's variable  
*/    


void I2C_ReadRegister (unsigned char u8Address, unsigned char u8Register, unsigned char *u8Data );

void ledInit(void);




#endif