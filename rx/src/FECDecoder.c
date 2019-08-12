#include"FECDecoder.h"

uint_8 decoder(uint_16 data , uint_8 enable)
{
	uint_8 result = data & 0x00ff;
	if(enable)
	{
	uint_8 h[5];
	h[0] = (data ^ (data>>1) ^ (data>>3) ^ (data>>5) ^ (data>>7) ^ (data>>8)) & 0x0001;
	h[1] = (data ^ (data>>2) ^ (data>>3) ^ (data>>6) ^ (data>>7) ^ (data>>9)) & 0x0001;
	h[2] = (data ^ (data>>4) ^ (data>>5) ^ (data>>6) ^ (data>>7) ^ (data>>10)) & 0x0001;
	h[3] = ((data>>1) ^ (data>>2) ^ (data>>3) ^ (data>>4) ^ (data>>5) ^ (data>>6) ^ (data>>7) ^ (data>>11)) & 0x0001;
	h[4] = (data ^ (data>>1) ^ (data>>2) ^ (data>>3) ^ (data>>4) ^ (data>>5) ^ (data>>6) ^ (data>>7) ^ (data>>8) ^ (data>>9) ^ (data>>10) ^ (data>>11) ^ (data>>12)) & 0x0001;
	uint_8 checkData = ( (h[4] & !h[3] & h[2] & h[1] & h[0]) |
			((h[4] & h[3] & !h[2] & !h[1] & h[0]) << 1) |
			((h[4] & h[3] & !h[2] & h[1] & !h[0]) << 2) |
			((h[4] & h[3] & !h[2] & h[1] & h[0]) << 3) |
			((h[4] & h[3] & h[2] & !h[1] & !h[0]) << 4) |
			((h[4] & h[3] & h[2] & !h[1] & h[0]) << 5) |
			((h[4] & h[3] & h[2] & h[1] & !h[0]) << 6) |
			((h[4] & h[3] & h[2] & h[1] & h[0]) << 7) );
	result = result ^ checkData;
	}
	return result;
}
