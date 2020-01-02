#include "I2C.h"
char znaki[] = {'x','A', 'B', 'C', 'D'}; 
char znakiASCII[] = {120, 65, 66, 67, 68};

void delay(void)
	{
		uint32_t i;
		for(i =1;i<100;i++)
			{}
	}


void I2C_Init(void)
{	
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK ;     	//zegar na I2C

	// Setting Pins corresponding to I2C0 clock and Data
	PORTE->PCR[24]|= PORT_PCR_MUX(5);
	PORTE->PCR[25]|= PORT_PCR_MUX(5);

	// Enabling pullup resistor I2C0 Data
	PORTE->PCR[24]|= PORT_PCR_PS_MASK |PORT_PCR_PE_MASK;
	// Enabling pullup resistor I2C0 Data
	PORTE->PCR[25]|= PORT_PCR_PS_MASK |PORT_PCR_PE_MASK;
	
	// Setting MULT to 2h
	I2C0->F|=I2C_F_MULT(2);
	I2C0->F|=I2C_F_ICR(0x00);
	
	// Enable the Module operation
	I2C0->C1|=I2C_C1_IICEN_MASK;
}

void Status_ACK_NACK(void)
{
	unsigned char AdressSlave= 0x1D;         // adress do wysylki 
	uint16_t adresssslave= 0x1D;
	char AdressSlave2[]= {0,'x',1,'D'};   //ten sam adres rozbity na znaki
  int a, j, k;	  

	uint16_t i;
  unsigned char NACK[] = {78, 65, 67, 75};   //zakodowane N A C K  w ascii
	unsigned char ACK[] = {65, 67, 75};	       //zakodowane A C K w ascii

//	for(a=0; a<strlen(AdressSlave2); a++)    // konwersja adresu do wysylki nei wiem czemu nei dziala jesli jest A to konweruje na 65 i wysyla 65 dzieki czemu widac w ANSII w konsoli
//	{
//		for(j=0; j<strlen(znaki); j++)
//		{
//			if(AdressSlave2[a] == znaki[j])
//			{
//				AdressSlave2[a]=znakiASCII[j];
//			}
//		}
//	}
//	

	for(k=0; k<strlen(AdressSlave2); k++)   // wysylka adresu
	{
		uint8_t p = AdressSlave2[k] - '0';
		UART0_Transmit(p);
	}
	
	
	
	UART0_Transmit(32);                   //spacja dla czytelnosci
	
	if(I2C_AddressRead(AdressSlave) == 255)   //warunek czy odpowiada czy nie NACK - nie  ACK - tak
	{
		for(i=0; i<4; i++)
			{
				UART0_Transmit(NACK[i]);
			}
	}
	else
	{
				for(i=0; i<3; i++)
			{
				UART0_Transmit(ACK[i]);
			}
	}


	
}


unsigned char I2C_AddressRead(unsigned char DEV_ADR/*, unsigned char REG_ADR*/)
{   
	unsigned char DATA_DUMMY =0;
	unsigned char DATA =0;
	uint32_t timeout = 99999;
	//Set I2C in Transmit mode
	I2C0->C1 |=I2C_C1_TX_MASK;
	// Send Start bit
	I2C0->C1 |=I2C_C1_MST_MASK;
	// Sending Device Address Write Bit as the last Bit
	I2C0->D = WRITE(DEV_ADR);
	//Wait for Transfer to complete i.e. until IICIF Flag is set to 0
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
	{
		timeout--;
		if(timeout == 0)
		{
			return 255;
		}
	}
	timeout = 99999;
	//Clear IICIF Flag
	I2C0->S |= I2C_S_IICIF_MASK;
	//Waiting for Acknowledgement from slave
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
	{
		timeout--;
		if(timeout == 0)
		{
			return 255;
		}
	}
	
	
	I2C0->C1 &= (~I2C_C1_MST_MASK);
	// Clear Transmit Nack by setting TXAK to 0
	I2C0->C1 &= ~(I2C_C1_TXAK_MASK);
	
	
			
////	// Sending Register Address of Magnetometer that we want to read
//	I2C0->D = 0x00;
////	//Wait for Transfer to complete i.e. until IICIF Flag is set to 0
//	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
//		{}
////	//Clear IICIF Flag
//	I2C0->S |= I2C_S_IICIF_MASK;
////	//Waiting for Acknowledgement from slave
//	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
//		{}
////	// Send the Repeated Start
//	I2C0->C1 |= I2C_C1_RSTA_MASK;
////	// Sending Device Address of the Magnetometer and a Read Bit as the last Bit
//	I2C0->D = READ(DEV_ADR);
////	//Sending NAK
//	I2C0->C1 |= (I2C_C1_TXAK_MASK);
////	//Wait for Transfer to complete i.e. until IICIF Flag is set to 0
//    while((I2C0->S & I2C_S_IICIF_MASK) == 0)
//		{}
////    //Clear IICIF Flag
//	I2C0->S|= I2C_S_IICIF_MASK;
////	//Waiting for Acknowledgement from slave
//	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
//		{}
////	//Set the I2C in Receiver Mode
//	I2C0->C1 &= (~I2C_C1_TX_MASK);
////	//Read Dummy Magnetometer Data
//	DATA_DUMMY = I2C0->D;
////	//Wait for Transfer to complete i.e. until IICIF Flag is set to 0
//	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
//		{}
////	//Clear IICIF Flag
//	I2C0->S |= I2C_S_IICIF_MASK;
////	//Send Stop Bit
//	I2C0->C1 &= (~I2C_C1_MST_MASK);
////	// Clear Transmit Nack by setting TXAK to 0
//	I2C0->C1 &= ~(I2C_C1_TXAK_MASK);
////	//Read Magnetometer Data
//	DATA= I2C0->D;
////	//delay to ensure all processes are completed before next process starts
//	delay();
//	//Return Data
	return 0;
}

unsigned char I2C_ReadSingle_Byte(unsigned char DEV_ADR, unsigned char REG_ADR)
{   
	unsigned char DATA_DUMMY =0;
	unsigned char DATA =0;
	uint32_t timeout = 99999;
	//Set I2C in Transmit mode
	I2C0->C1 |=I2C_C1_TX_MASK;
	// Send Start bit
	I2C0->C1 |=I2C_C1_MST_MASK;
	// Sending Device Address Write Bit as the last Bit
	I2C0->D = WRITE(DEV_ADR);
	//Wait for Transfer to complete i.e. until IICIF Flag is set to 0
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
	{
		timeout--;
		if(timeout == 0)
		{
			return 255;
		}
	}
	timeout = 99999;
	//Clear IICIF Flag
	I2C0->S |= I2C_S_IICIF_MASK;
	//Waiting for Acknowledgement from slave
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
	{
		timeout--;
		if(timeout == 0)
		{
			return 255;
		}
	}
	
	
	I2C0->C1 &= (~I2C_C1_MST_MASK);
	// Clear Transmit Nack by setting TXAK to 0
	I2C0->C1 &= ~(I2C_C1_TXAK_MASK);
	
	
			
//	// Sending Register Address of Magnetometer that we want to read
	I2C0->D = REG_ADR;
//	//Wait for Transfer to complete i.e. until IICIF Flag is set to 0
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{
			timeout--;
			if(timeout == 0)
			{
				return 255;
			}
		}
			timeout = 99999;
//	//Clear IICIF Flag
	I2C0->S |= I2C_S_IICIF_MASK;
//	//Waiting for Acknowledgement from slave
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{
			timeout--;
				if(timeout == 0)
				{
					return 255;
				}
		}
		timeout = 99999;
//	// Send the Repeated Start
	I2C0->C1 |= I2C_C1_RSTA_MASK;
//	// Sending Device Address of the Magnetometer and a Read Bit as the last Bit
	I2C0->D = READ(DEV_ADR);
//	//Sending NAK
	I2C0->C1 |= (I2C_C1_TXAK_MASK);
//	//Wait for Transfer to complete i.e. until IICIF Flag is set to 0
    while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{
		timeout--;
			if(timeout == 0)
			{
				return 255;
			}
		}
//    //Clear IICIF Flag
	I2C0->S|= I2C_S_IICIF_MASK;
//	//Waiting for Acknowledgement from slave
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{
			timeout--;
				if(timeout == 0)
				{
					return 255;
				}
		}
//	//Set the I2C in Receiver Mode
	I2C0->C1 &= (~I2C_C1_TX_MASK);
//	//Read Dummy Magnetometer Data
	DATA_DUMMY = I2C0->D;
//	//Wait for Transfer to complete i.e. until IICIF Flag is set to 0
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
//	//Clear IICIF Flag
	I2C0->S |= I2C_S_IICIF_MASK;
//	//Send Stop Bit
	I2C0->C1 &= (~I2C_C1_MST_MASK);
//	// Clear Transmit Nack by setting TXAK to 0
	I2C0->C1 &= ~(I2C_C1_TXAK_MASK);
//	//Read Magnetometer Data
	DATA= I2C0->D;
//	//delay to ensure all processes are completed before next process starts
	delay();
	//Return Data
	return DATA;
}
