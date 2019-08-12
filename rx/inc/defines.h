#include "msp432.h"
#include "driverlib.h"

#ifndef DEFINES_H_
#define DEFINES_H_

typedef unsigned char   uint_8;
typedef unsigned short  uint_16;
typedef unsigned int    uint_32;
typedef char            int_8;
typedef short           int_16;
typedef int             int_32;

#define IDLE			0
#define ADC14INT		1
#define RECEIVEBEGIN	2
#define RECEIVEDONE		3

#define ADC_THRESHOLD	0x1000
#define STOPTHRESHOLD	2000
#define THRESHOLD		1800000.0f//0.18f

#define DATA_LENGTH_BYTE		300
#define MODULATE_DUTY			1
#define SAMPLE					4//(MODULATE_DUTY << 2)
#define TOTAL_SAMPLES			(DATA_LENGTH_BYTE * SAMPLE * 13)//13 for FEC
#define BYTE_SAMPLES			(SAMPLE * 13)
#define OFFSET					1

#define TASKIDLE				0
#define STARTSAMPLE				1
#define ENDSAMPLE				2
#define TASKEND					3

#define CS_HFXTFREQ		48000000
#define CS_LFXTFREQ		32768

typedef struct _Send_Info
{
	uint_32 data;
	uint_8 length;
	uint_8 counter;
	uint_8 flag;
} Send_Info;

#endif /* DEFINES_H_ */
