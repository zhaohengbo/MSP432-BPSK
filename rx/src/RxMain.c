#include "defines.h"
#include "System.h"
#include "arm_math.h"
#include "stdio.h"
#include "string.h"
#include "FECDecoder.h"

extern void srand(short);
extern short rand(void);
extern short time();

int16_t receiveBuffer[TOTAL_SAMPLES + OFFSET];
uint_16 recoveryData[DATA_LENGTH_BYTE];

volatile uint_16 index = 0;
volatile uint_32 cpuCounter = 0;
volatile uint_8 task = 0;
uint_16 temp = 0;
uint_16 stopIndex = 0;

float storage1[BYTE_SAMPLES];
float storage1FIR[BYTE_SAMPLES];
float storage2[BYTE_SAMPLES];
float carrier[BYTE_SAMPLES];
float carrierFIR[BYTE_SAMPLES];
float *testInput;
float *productPtr;
uint_8 bitError[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//for FEC test

#define FIRSTAGE	24

float firCoeffs32[FIRSTAGE] = {0.000682951864720116f,	0.00237583065581085f,	-0.00397621327812468f,	-0.00261057734866603f,
		0.0139097044819179f,	-0.00674246551919615f,	-0.0267020326609336f,	0.0396413200784337f,
		0.0225877521713499f,	-0.113968371926128f,	0.0629570513530337f,	0.511845050127782f,
		0.511845050127782f,	0.0629570513530337f,	-0.113968371926128f,	0.0225877521713499f,
		0.0396413200784337f,	-0.0267020326609336f,	-0.00674246551919615f,	0.0139097044819179f,
		-0.00261057734866603f,	-0.00397621327812468f,	0.00237583065581085f,	0.000682951864720116f};

static float32_t firStateF32[BYTE_SAMPLES + FIRSTAGE - 1];

void main(void)
{
	MAP_WDT_A_holdTimer();
	systemClockInit();
	srand( (unsigned)time( NULL ) );//random seed
	volatile uint32_t i = 0;

	timerA0Init();// for adc sample
	timerA1Init();// for runtime calculating
	ADC14Init();
	ADC14->CTL0 |= ADC14_CTL0_ENC;	//enable ADC
	TA0CTL |= MC__UP;		//start ADC sample

	stopIndex = 0;
	uint_8 j = 0;
	task = TASKEND;

	index = 0;

    arm_fir_instance_f32 S;
    uint_16 FECDataOld;
    uint_16 FECDataNew;
    arm_fir_init_f32(&S, FIRSTAGE, (float32_t *)&firCoeffs32[0], &firStateF32[0], BYTE_SAMPLES);

	MAP_Interrupt_enableMaster();
	while(1)
	{
		if(task == STARTSAMPLE)
		{
			stopIndex = index;
			if(stopIndex > 6)
			{
				if( (receiveBuffer[stopIndex-1] + receiveBuffer[stopIndex-2] + receiveBuffer[stopIndex-3] +
					receiveBuffer[stopIndex-4] + receiveBuffer[stopIndex-5] + receiveBuffer[stopIndex]) < STOPTHRESHOLD)//sample end condition
				{
					task = ENDSAMPLE;
					TA0CTL = TASSEL__SMCLK | TACLR;
				}
			}
		}
		if(task == ENDSAMPLE)
		{
			TA1CTL |= MC__UP;
			int16_t *ptr;
			task = TASKEND;
			if(stopIndex > BYTE_SAMPLES)
			{
				ptr = &receiveBuffer[OFFSET];

				arm_q15_to_float(ptr , carrier , BYTE_SAMPLES);		//float type carrier
				productPtr = &carrier[0];
				for(i=0;i<=stopIndex/BYTE_SAMPLES;i++)
				{
					FECDataNew = FECDataOld >> 3;
					FECDataOld = 0;
					testInput = &storage1[0];
					ptr = &receiveBuffer[OFFSET+i*BYTE_SAMPLES];
					arm_q15_to_float(ptr , testInput , BYTE_SAMPLES);
					testInput = &storage1[0];
					productPtr = &storage1[0];
					testInput = &storage1FIR[0];
					arm_fir_f32(&S, productPtr, testInput, BYTE_SAMPLES);//24-stage filter
					recoveryData[i] = 0;
					testInput = &storage1FIR[0];
					productPtr = &storage2[0];
					//coherent demodulation
					arm_mult_f32(testInput , &carrier[0], productPtr , BYTE_SAMPLES);
					for(j=0;j<13;j++)
					{
						FECDataOld |= ((storage2[2+j*SAMPLE] < THRESHOLD) ? 0x01 : 0x00) << j;
					}
					FECDataNew |= (FECDataOld << 10);//for 11.5 phase delay, when using 24-stage FIR
					recoveryData[i] = decoder(FECDataNew , 1);
				}
			}
		}
		if(task == TASKEND)
		{
			TA1CTL = TASSEL__ACLK;
			TA1CTL |= TACLR;
			for(i = 0;i<DATA_LENGTH_BYTE;i++)
				recoveryData[i] = 0;
			for(index = 0;index < (TOTAL_SAMPLES + OFFSET);index++)//add noise, noise need to be updated
				receiveBuffer[index] = 0;
			index = 0;
			i = 0;
			j = 0;
			ADC14->IER0 &= ~ADC14_IER0_IE0;			//disable ADC0 interrupt
			ADC14->IER1 = ADC14_IER1_HIIE;			//enable ADC HI-INT
			TA0CTL |= MC__UP;
			task = TASKIDLE;
		}
	}
}

void ADC14_IRQHandler(void)
{
	temp = ADC14->IV;
	switch(temp)
	{
	case 0x06:
		ADC14->IER1 &= ~ADC14_IER1_HIIE;		//disable threshold function
		ADC14->IER0 = ADC14_IER0_IE0;			//enable ADC0 interrupt
		break;

	case 0x0c:
		receiveBuffer[index] = ADC14->MEM[0];
		index++;
		task = STARTSAMPLE;
		break;

	default: break;

	}
}

void TA1_0_IRQHandler(void)
{
	TA1CCTL0 &= ~CCIFG;
	cpuCounter = 0;
}
