#include "System.h"

void systemClockInit(void)
{
	MAP_CS_setExternalClockSourceFrequency(32768 , 48000000);//LFXT = 32.768kHz, HFXT = 48MHz
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ ,
				GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 ,
				GPIO_PRIMARY_MODULE_FUNCTION); //PJ.0-PJ.3 used as XT

	//Setting for 48MHz CPU clock
	MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
	MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
	MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
	MAP_CS_startHFXT(false);

	MAP_CS_initClockSignal(CS_ACLK , CS_LFXTCLK_SELECT , CS_CLOCK_DIVIDER_1);		//ACLK = LFXT 32kHz
	MAP_CS_initClockSignal(CS_MCLK , CS_HFXTCLK_SELECT , CS_CLOCK_DIVIDER_1);		//MCLK = HFXT 48MHz
	MAP_CS_initClockSignal(CS_HSMCLK , CS_HFXTCLK_SELECT , CS_CLOCK_DIVIDER_1);		//HSMCLK = HFXT 48MHz
	MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_16);		//SMCLK =  HFXT/16 3MHz
}

void ADC14Init(void)
{
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,
		    GPIO_TERTIARY_MODULE_FUNCTION);		//P5.5 used as A0 input
	ADC14->CTL0 = ADC14_CTL0_ON;	//enable module
	ADC14->CTL0 |= ADC14_CTL0_SHS0 | ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_CONSEQ_2;	//TA0_1 trigger, MCLK, repeat-singal-channel
	ADC14->CTL1 = ADC14_CTL1_RES__14BIT;		//14-bit
	ADC14->MCTL[0] = ADC14_MCTLN_WINC;
	ADC14->HI0 = ADC_THRESHOLD;	//~0.8v
	ADC14->IER1 = ADC14_IER1_HIIE;
	Interrupt_enableInterrupt(INT_ADC14);
}

void timerA0Init(void)//use TA0_1 as trigger
{
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
	            GPIO_PRIMARY_MODULE_FUNCTION);
	TA0CTL = TASSEL__SMCLK | TACLR;
	TA0CCR0 = 5;	//3M / 6 = 500kHz, for sample rate
	TA0CCR1 = 3;	//duty cycle
	TA0CCTL1 = OUTMOD_2;
}

void timerA1Init(void)//use TA1_0 for cpu counter
{
	TA1CTL = TASSEL__ACLK | TACLR;
	TA1CCTL0 = CCIE;
	TA1CCR0 = 0xffff;
}
