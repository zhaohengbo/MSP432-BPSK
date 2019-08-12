#include "Modulation.h"

void startCarrier(void) {TA0CTL |= MC__UP | TACLR;}
void stopCarrier(void) {TA0CTL = TASSEL__SMCLK | ID__2 | TACLR;}
void startSignal(void) {TA1CTL |= MC__UP | TACLR;}
void stopSignal(void) {TA1CTL = TASSEL__SMCLK | ID__2 | TACLR;}

void setBitRate(uint_8 data)
{
	TA1CTL |= TACLR;
	TA1CCR0 = data;
}

void modulateOneBit(uint_8 data)
{
	if((data & 0x01) == 0x01)
		SYMBOL_1;
	else
		SYMBOL_0;
}

void initSendInfo(Send_Info *sendInfo , uint_8 enableFEC)
{
	(*sendInfo).counter = 0;
	(*sendInfo).flag = 0;
	if(enableFEC)
		(*sendInfo).length = 13;
	else
		(*sendInfo).length = 8;
}
