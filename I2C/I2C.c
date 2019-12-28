#include "I2C.h"

#define addressACCELEROMETER  0x1D  //read -> 0x3A       write -> 0x3B
#define addressRegisters  0x00 // do 0x06 to sa LSB I MSB dla osi XYZ

#define bus_clock 8000000    


#define MWSR 0x00 /* Master write */    
#define MRSW 0x01 /* Master read */ 

void I2C_EnableACK(){ I2C1->C1 &= ~I2C_C1_TXAK_MASK; }
void I2C_DisableACK_NACK() { I2C1->C1 |= I2C_C1_TXAK_MASK; }
void I2C_Start() { I2C0->C1 |= I2C_C1_MST_MASK;  }
void I2C_TRAN() { I2C0->C1 |= I2C_C1_TX_MASK;}
void I2C_REC() {I2C0->C1 &= ~I2C_C1_TX_MASK; }
void I2C_RepeatedStart() {I2C0->C1 |= I2C_C1_RSTA_MASK; }
void I2C_Stop() { I2C0->C1 &= ~I2C_C1_MST_MASK; }

void Busy_ACK() {while(I2C0->S & 0x01); }
void Tran_Comp() { while(!(I2C0->S & 0x80)); }
void I2C_wait() {while((I2C0->S & I2C_S_IICIF_MASK)==0){} \
												I2C0->S |= I2C_S_IICIF_MASK; }
void NACK() {I2C0->C1 |= I2C_C1_TXAK_MASK ;}
void ACK() {I2C0->C1 &= ~I2C_C1_TXAK_MASK;}



void I2C_EnterRxMode()
{
	I2C1->C1 &= ~I2C_C1_TX_MASK;		//RECEIVE MODE
  I2C1->C1 |= I2C_C1_TXAK_MASK;   //TRANSMIT ACKNOWLEDGE ENABLE
}	

void I2C_write_byte(uint32_t data)
{
	I2C1->D = data;
}

char I2C_read_byte()
{
	return I2C1->D;
}






void I2Cinit(void)
{
	uint32_t baud_rate;
	uint32_t divisor;
	
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;// | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTB_MASK;      //NA PORT
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK ;     //zegar na I2C
	
	
	PORTE->PCR[24] |= PORT_PCR_MUX(2);						//  PINY DOLACZONE DO MODULU I2C 24- SCL , 25- SDA 
  PORTE->PCR[25] |= PORT_PCR_MUX(2) ;
	
	//PORTE->PCR[24] |= PORT_PCR_PE_MASK |	PORT_PCR_PS_MASK;				//  
  //PORTE->PCR[25] |= PORT_PCR_PE_MASK |	PORT_PCR_PS_MASK;
	
	
	//I2C0 ->F = I2C_F_ICR_MASK;               //
	//divisor = bus_clock/(baud_rate*16);   //dzielnik
  //I2C0->C2 &= !I2C_C2_ADEXT_MASK;								

	
//	I2C0 -> F |= I2C_F_MULT(0x01);
//	I2C0 -> F |= I2C_F_ICR(0x28);
I2C0 -> F |= I2C_F_MULT(2);
	
	//I2C0->F = 0x24; //BAUDRATE 
	//I2C0-> F = 0X1E;   // For 100 Khz
	//I2C_F = 0x0E // For 400Khz
 // I2C_F = 0X02 // For 1Mhz respectively.
	//I2C0->SLTH = I2C_SLTH_SSLT_MASK;                //set SCL  100k
	//I2C0->SLTL = I2C_SLTL_SSLT_MASK;
	
	
	I2C0 ->C1 |= I2C_C1_IICEN_MASK;    //ENABLE I2C
  I2C0->C1 |= I2C_C1_IICIE_MASK; /*enable interrupt for timing signals*/	
	
 // I2C0->C1 |= I2C_C1_TXAK_MASK;
//	I2C0->C1 |= I2C_C1_MST_MASK;										
	I2C0->A1 |= I2C_A1_AD(addressACCELEROMETER);   //default address of accelerator id 0x1D

}



void Transmite(uint8_t data)
{
		

		I2C0 ->C1 |= I2C_C1_TX_MASK;         //ustawianie na wysylanie
		I2C0->D = addressACCELEROMETER; /*send dev address */
	 /* dodac ? wait for ack */
		I2C0->D = addressRegisters; /*send write address */
	 /* dodac ? wait for ack */
    I2C0->D = data; /*send data */
		/* dodac ? wait for ack */
}

uint8_t Reading_single_Byte_from_Device(void)   //funkjca oparta na pdfie ARMa jak zadziala przerobic multiple na podawanie adresów
{
	uint16_t data =0;
	volatile int d;
	
	I2C_TRAN();       //set to transmit mode 
	I2C_Start();       //send start 
	I2C0->D=(addressACCELEROMETER);   //send dev address 
	I2C_wait();                 //wait for completion 
	I2C0->D = addressRegisters;      //send read address 
	I2C_wait();                 //wait for completion 
	I2C_RepeatedStart();             //repeated start 
	I2C0->D = ((addressACCELEROMETER<<1)|0x1); //send dev address (read) 
	I2C_wait();                 //wait for completion 
	I2C_REC();                 //set to recieve mode 
	NACK();                //set NACK after read 
	data = I2C0->D;           //dummy read 
  I2C_wait();           //wait for completion 
  I2C_Stop();             //send stop 
  data = I2C0->D;        //read data 
	return data;

}

void Reading_Multiple_Bytes_set_up(void) //NA PODSTAWIE  pdfARMA
{
	I2C_TRAN();       //set to transmit mode 
	I2C_Start();      //send start 
	I2C0->D=addressACCELEROMETER;   //send dev address 
	I2C_wait();                 //wait for completion 
	I2C0->D = addressRegisters;      //send read address  // co to jest jaki adres tu podawac 
	I2C_wait();                 //wait for completion 
	I2C_RepeatedStart();             //repeated start 
	I2C0->D = (addressACCELEROMETER|0x1); //send dev address (read) 
	I2C_wait();                 //wait for completion 
	I2C_REC();                 //set to recieve mode 
}

 void Master_read_Slave_write_SET_up(void)
 {
	I2C_TRAN();       //set to transmit mode 
	I2C_Start();       //send start 
	I2C0->D = (addressACCELEROMETER|0x0); //send dev address (read) 
	//I2C_wait();                 //wait for completion
	ACK();
	I2C0->D = addressRegisters;      //send read address // co to jest jaki adres tu podawac 
	//I2C_wait();                 //wait for completion 
	ACK();
	I2C_TRAN();       //set to transmit mode 
	I2C_Start();       //send start 
	

	 
 }
 
 
 
 /* -------------------------------------------------------------------
 -------------------------------------------------------------------------*/
 

 /*
 unsigned char I2C_ReadRegister_uc (unsigned char u8Address, unsigned char u8Register ){    
  
	 unsigned char u8Data;    
  unsigned char u8AddressW, u8AddressR;    
  // shift ID in right possition     
  u8AddressW = (u8Address << 1) | MWSR; // write address MASTER WRITES SLAVE READS      
  u8AddressR = (u8Address << 1) | MRSW; // read address  MASTER READS SLAVE WRITES      
  // send start signal 
  I2C_TRAN();       //set to transmit mode 	 
  I2C_Start(); 
  // send ID with Write bit    
  I2C_write_byte(u8AddressW);    
  I2C_Wait();    
  // send Register address    
  I2C_write_byte(u8Register);    
  I2C_Wait();    
  // send repeated start to switch to read mode    
  I2C_RepeatedStart();    
  // re send device address with read bit    
  I2C_write_byte(u8AddressR);    
  I2C_Wait();    
  // set in read mode    
  I2C_EnterRxMode();    
  u8Data = I2C_read_byte();    
  // send stop signal so we only read 8 bits    
  I2C_Stop();    
  return u8Data;    
}    
/*  
* Name: I2C_ReadRegister  
* Requires: Device Address, Device Register address, Pointer for returned data  
* Returns: nothing  
* Description: Reads device register and puts it in pointer's variable  
*/    

/*
void I2C_ReadRegister (unsigned char u8Address, unsigned char u8Register, unsigned char *u8Data ){    
  // shift ID in right possition     
  u8Address = (u8Address << 1) | MWSR; // write address MASTER WRITES SLAVE READS  
  u8Address = (u8Address << 1) | MRSW; // read address  MASTER READS SLAVE WRITES  
  // send start signal 
	I2C_TRAN();       //set to transmit mode 	
  I2C_Start();    
  // send ID with W bit     
  I2C_write_byte(u8Address);    
  I2C_Wait();    
  // send device register    
  I2C_write_byte(u8Register);    
  I2C_Wait();    
  // repeated start for read mode    
  I2C_RepeatedStart();    
  // resend device address for reading    
  I2C_write_byte(u8Address);    
  I2C_Wait();     
  I2C_EnterRxMode();    
  // clear data register for reading    
  *u8Data = I2C_read_byte();    
  I2C_Wait();    
  // send stop signal so we only read 8 bits    
  I2C_Stop();      
}    

*/

void ledInit(void){
	

  int d=0; 
	SIM->SCGC5 |=  SIM_SCGC5_PORTE_MASK; 	
	PORTE->PCR[29] = PORT_PCR_MUX(1);    
	FPTE->PDDR = 1UL<<29;
	FPTE->PCOR = 1UL<<29;
	for(d=0; d<1200000; d++);
	FPTE->PSOR = 1UL<<29	;          
	d=0;

SIM->SCGC5 |=  SIM_SCGC5_PORTD_MASK; 	
	PORTD->PCR[5] = PORT_PCR_MUX(1);    
	FPTD->PDDR = 1UL<<5;
	FPTD->PCOR = 1UL<<5;
	for(d=0; d<1200000; d++);
	FPTD->PSOR = 1UL<<5	;          
	
}