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

#define CS_HFXTFREQ		48000000
#define CS_LFXTFREQ		32768

#define BIT_RATE_125000         5
#define BIT_RATE_62500          11
#define BIT_RATE_31250          23
#define BIT_RATE_25000          29
#define BIT_RATE_12500          59
#define BIT_RATE_5000           149
#define BIT_RATE_1000           749

#define SYMBOL_1	TA0CCTL2 = OUTMOD_6
#define SYMBOL_0	TA0CCTL2 = OUTMOD_2

#define ENABLE_FEC	1
#define DISABLE_FEC	0

typedef struct _Send_Info
{
	uint_32 data;
	uint_8 length;
	uint_8 counter;
	uint_8 flag;
} Send_Info;

#endif /* DEFINES_H_ */
