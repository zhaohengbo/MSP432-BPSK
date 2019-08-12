#include "FECEncoder.h"

uint_16 encoder(uint_8 data , uint_8 enable)
{
	uint_16 result = data & 0x00ff;
	if(enable)
	{
	uint_16 h[5];
	h[0] = (data ^ (data>>1) ^ (data>>3) ^ (data>>5) ^ (data>>7)) & 0x0001;
	h[1] = (data ^ (data>>2) ^ (data>>3) ^ (data>>6) ^ (data>>7)) & 0x0001;
	h[2] = (data ^ (data>>4) ^ (data>>5) ^ (data>>6) ^ (data>>7)) & 0x0001;
	h[3] = ((data>>1) ^ (data>>2) ^ (data>>3) ^ (data>>4) ^(data>>5) ^ (data>>6) ^ (data>>7)) & 0x0001;
	h[4] = ((data>>1) ^ (data>>2) ^ (data>>4) ^ (data>>7)) & 0x0001;
	result = result | (h[0] << 8) | (h[1] << 9) | (h[2] << 10) | (h[3] << 11) | (h[4] << 12);
	}
	return result;
}
