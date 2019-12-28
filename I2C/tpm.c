#include "MKL46Z4.h"                    	
#include "tpm.h"


	
//	uint16_t counter = 0;
	
	
void TpmInitialize(void){
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;	//zalaczenie zegara dla TPM0
	
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(3); //SIM_SOPT2_TPMSRC_MASK	//wybor tpm source as Multipurpose Clock Generator Internal Reference Clock'
																			//MCGIRCLK = 0b11 = 3
	//SIM->SOPT2|=SIM_SOPT2_PLLFLLSEL_MASK;
	
	MCG->C1 |=MCG_C1_IREFS_MASK;
	MCG->C1|=MCG_C1_IRCLKEN_MASK;				//ustawienie czestotliwosci OSC32KCLK
	//TPM0->SC |=TPM_SC_TOF_MASK;
	//TPM0->SC |=TPM_SC_TOIE_MASK;
	TPM0->SC |=TPM_SC_PS(64);             //prescaler
	TPM0->CNT |=TPM_CNT_COUNT(16);       //licznik 
	
	//TPM0->CONTROLS[2].CnSC|=(TPM_CnSC_MSB_MASK
		//										| TPM_CnSC_ELSB_MASK);
			//									//| TPM_CnSC_ELSA_MASK);
	//TPM0->CONTROLS[2].CnV|=TPM_CnV_VAL(0xFFF) ;
	//TPM0->CONTROLS[0].CnSC|=TPM_CnSC_CHF_MASK;	//wyczyszczenie flagi
	//TPM0->CONTROLS[0].CnSC|=TPM_CnSC_CHIE_MASK; //Enable interrupt, odblokowanie kanalu timera
	//TPM0->STATUS |= TPM_STATUS_CH2F_MASK;
	
	TPM0->SC |=TPM_SC_CMOD(0x01); //TMP increments rising edge
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;
	TPM0->MOD |= TPM_MOD_MOD(0x0D);
	TPM0->CONTROLS[0].CnSC |= TPM_CnSC_MSA_MASK  | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSA_MASK;
	
	
	NVIC_ClearPendingIRQ(TPM0_IRQn);		//wyczyszczenie 
	NVIC_EnableIRQ(TPM0_IRQn);					//odblokowanie przerwan
	NVIC_SetPriority (TPM0_IRQn, 3);		//ustawienie priorytetow
	TPM0->CONTROLS[0].CnSC |= TPM_CnSC_CHIE_MASK ;
	
}

void TPM0_IRQHandler(void)
{

//	TPM0->CONTROLS[0].CnSC|= TPM_CnSC_CHIE_MASK;   //enable interrupts
	PTE->PTOR = (1UL << 29);   								 
	TPM0->STATUS |= TPM_STATUS_CH0F_MASK;   // wystapilo zdarzenie kanalu na kanael 0
	TPM0->CONTROLS[0].CnSC|=TPM_CnSC_CHF_MASK;
//	zmiana stanu na diodzie
	
 
}