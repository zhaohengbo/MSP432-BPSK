#include "defines.h"
#include "Packet.h"
#include "System.h"
#include "stdio.h"
#include "stdlib.h"
#include "Modulation.h"

uint_8 forDebug = 0;
Send_Info instSendInfo =
{
	.length = 0,
	.counter = 0,
	.flag = 0
};

Packet sendTestData =
{
	.delimiter = 0b10000010,
	.addr = 0x12345678,
	.command = 1,
	.dataLength = 191
};

uint_8 sendBitFlag = 0;
volatile uint_8 intFlag = 0;
uint_32 cpuCounter = 0;

void main(void)
{
    WDT_A_holdTimer();           // Stop watchdog timer
    uint_16 i;
    systemClockInit();
    initP1ISR();
    uint_8 sendData[200];
    for(i = 0;i<200;i++)
    	sendData[i] = i;

    sendTestData.data = &sendData[0];

    uint_16 buffer[530];
    uint_16 *sendBuffer;

    timerA0Init();
    timerA1Init();
    setBitRate(BIT_RATE_125000);

    sendBuffer = buffer;

    initSendInfo(&instSendInfo , ENABLE_FEC);
    sendBitFlag = 0;

    sendPacket(&sendTestData , sendBuffer , ENABLE_FEC);

    uint_16 packetLength = (*sendBuffer) & 0x00ff;
    sendBuffer++;
    packetLength = (packetLength << 8) | ((*sendBuffer) & 0x00ff);

    sendBuffer++;
    sendBitFlag = 1;
    packetLength = 0;

    //TA2CTL |= MC__UP;

	Interrupt_enableMaster();
	cpuCounter = 0;

    while(1)//
    {
    	if(intFlag)
    	{
    		intFlag = 0;
    		GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);//enable carrier output
    		startCarrier();
			startSignal();
    	}

		if(sendBitFlag)//send interrupt flag
		{
			sendBitFlag = 0;
			if(packetLength != 0)
			{
				if(instSendInfo.counter == instSendInfo.length)
				{
					sendBuffer++;
					instSendInfo.counter = 0;
					packetLength--;
				}
				modulateOneBit(*sendBuffer);//modulation
				*sendBuffer = *sendBuffer >> 1;
				instSendInfo.counter++;
			//send entry
			}
			else
			{//modulation stop
				stopSignal();
				stopCarrier();
				TA2CTL = TASSEL__SMCLK;
				TA2CTL |= TACLR;
				P1IE |= BIT1;
				GPIO_setAsOutputPin(GPIO_PORT_P2 , GPIO_PIN5);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2 , GPIO_PIN5);
				sendTestData.data = &sendData[0];
				sendBuffer = buffer;
				sendPacket(&sendTestData , sendBuffer , ENABLE_FEC);
				packetLength = (*sendBuffer)  & 0x00ff;
				sendBuffer++;
				packetLength = (packetLength << 8) | ((*sendBuffer) & 0x00ff);
				sendBuffer++;
			}
		}
    	else
    		cpuCounter++;
    }
}

void TA1_0_IRQHandler(void)
{
	TA1CCTL0 &= ~CCIFG;
	sendBitFlag = 1;
}

void TA2_0_IRQHandler(void)
{
	TA2CCTL0 &= ~CCIFG;
	cpuCounter = 0;
}

void PORT1_IRQHandler(void)
{
	intFlag = 1;
	P1IFG &= ~BIT1;
	P1IE &= ~BIT1;
	//cpuCounter = 0;
	//TA2CTL |= MC__UP;
}
