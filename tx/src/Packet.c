#include "Packet.h"

uint_8 formatPacket(Packet *packet)	//calculate check sum
{
	uint_16 i = 0;
	uint_8 checkSum = 0;
	checkSum ^= (*packet).delimiter;

	for(i=0;i<25;i+=8)
		checkSum ^= (*packet).addr >> i;
	for(i=0;i<9;i+=8)
		checkSum ^= (*packet).dataLength >> i;
	checkSum ^= (*packet).command;

	for(i=0;i<(*packet).dataLength;i++)
		checkSum ^= (*packet).data[i];
	return checkSum;
}

void sendPacket (Packet *packet , uint_16 *sendBuffer , uint_8 enableFEC)
{
	uint_16 i = 0;
	uint_8 checkSum;

	uint_16 sendLength = 19 + (*packet).dataLength;

	*sendBuffer++ = encoder(sendLength >> 8 , enableFEC);
	*sendBuffer++ = encoder(sendLength & 0x00ff , enableFEC);

	checkSum = formatPacket(packet);

	for(i=0;i<5;i++)							//5 Byte 0 for carrier sync
		*sendBuffer++ = encoder(0 , enableFEC);

	for(i=0;i<5;i++)							//preamble
		*sendBuffer++ = encoder(0xff , enableFEC);

	*sendBuffer++ = encoder((*packet).delimiter , enableFEC);		//delimiter
	for(i=0;i<25;i+=8)							//addr
		*sendBuffer++ = encoder((*packet).addr >> i , enableFEC);

	*sendBuffer++ = encoder((*packet).command , enableFEC);			//command
	*sendBuffer++ = encoder((*packet).dataLength & 0x00ff , enableFEC);	//length
	*sendBuffer++ = encoder((*packet).dataLength >> 8 , enableFEC);	//length
	for(i=0;i<(*packet).dataLength;i++)				//data
		*sendBuffer++ = encoder((*packet).data[i] , enableFEC);
	*sendBuffer = encoder(checkSum , enableFEC);				//checksum
}
