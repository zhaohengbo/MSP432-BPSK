#ifndef MODULATION_H_
#define MODULATION_H_

#include "defines.h"

//this function is to set modulated bit rate
// parameter is one of :
//        BIT_RATE_125000
//        BIT_RATE_62500
//        BIT_RATE_31250
//        BIT_RATE_25000
//        BIT_RATE_12500
//        BIT_RATE_5000
//        BIT_RATE_1000
void setBitRate(uint_8);

//this function is used to generate the carrier, according to the data's LSB
void modulateOneBit(uint_8);

//enable carrier's timer
void startCarrier(void);
//stop carrier's timer
void stopCarrier(void);
//enable signal's timer
void startSignal(void);
//stop signal's timer
void stopSignal(void);

//structuer Send_Info is used for data modulating
//this function is used to initialize this structure
//the first parameter is a reference of Send_Info
//the second parameter is for FEC enable
void initSendInfo(Send_Info * , uint_8);

#endif /* MODULATION_H_ */
