#include "System.h"

void systemClockInit(void)
{
	CS_setExternalClockSourceFrequency(32768 , 48000000);//LFXT = 32.768kHz, HFXT = 48MHz
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ ,
				GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 ,
				GPIO_PRIMARY_MODULE_FUNCTION); //PJ.0-PJ.3 used as XT

	//Setting for 48MHz CPU clock
	PCM_setCoreVoltageLevel(PCM_VCORE1);
	FlashCtl_setWaitState(FLASH_BANK0, 2);
	FlashCtl_setWaitState(FLASH_BANK1, 2);
	CS_startHFXT(false);

	CS_initClockSignal(CS_ACLK , CS_LFXTCLK_SELECT , CS_CLOCK_DIVIDER_1);		//ACLK = LFXT 32kHz
	CS_initClockSignal(CS_MCLK , CS_HFXTCLK_SELECT , CS_CLOCK_DIVIDER_1);		//MCLK = HFXT 48MHz
	CS_initClockSignal(CS_HSMCLK , CS_HFXTCLK_SELECT , CS_CLOCK_DIVIDER_1);		//HSMCLK = HFXT 48MHz
	CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_32);		//SMCLK =  HFXT/32 1.5MHz
}

void timerA0Init(void)//use TA0 as carrier, carrier frequency is 125kHz
{
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4 | GPIO_PIN5,
	            GPIO_PRIMARY_MODULE_FUNCTION);
	TA0CTL = TASSEL__SMCLK | ID__2 | TACLR;
	TA0CCR0 = 5;
	TA0CCR1 = 3;//channel 1 is just for monitor
	TA0CCR2 = 3;
	TA0CCTL1 = OUTMOD_2;
	TA0CCTL2 = OUTMOD_6;
}

void timerA1Init(void)//use TA1 as bit rate generator
{
	TA1CTL = TASSEL__SMCLK | ID__2 | TACLR;
	TA1CCTL0 = CCIE;
	Interrupt_enableInterrupt(INT_TA1_0);
}

void timerA2Init(void)//use TA2 for cpu calculate
{
	TA2CTL = TASSEL__SMCLK | TACLR;
	TA2CCTL0 = CCIE;
	TA2CCR0 = 0xb5d;
	Interrupt_enableInterrupt(INT_TA2_0);
}

void initP1ISR(void)
{
	P1OUT |= BIT1;
	P1DIR &= ~BIT1;
	P1REN |= BIT1;
	P1IE |= BIT1;
	P1IES |= BIT1;
	P1IFG &= ~BIT1;
	Interrupt_enableInterrupt(INT_PORT1);
}
