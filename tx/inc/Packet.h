#ifndef PACKET_H_
#define PACKET_H_

#include "defines.h"
#include "FECEncoder.h"

/****************************************************************************************
     5B       5B         1B        4B      1B        2B     0-492B      1B
  | Sync | Preamble | Delimiter | Addr | Command | Length |  Data   | Checksum |
*****************************************************************************************/
typedef struct _Packet //protocol packet format
{
	uint_8 delimiter; 		// delimiter field
	uint_32 addr; 		// address field, 5 byte
	uint_8 command;			// command field
	uint_16 dataLength;	// data length field
	uint_8 *data;           // data field
} Packet;

//this function format the packet. its parameter is a reference of variable packet
//and its return value is the check sum value of the packet, using XOR operation
uint_8 formatPacket(Packet *);//return check sum value

//this function sends the packet to send buffer, with the right order
void sendPacket (Packet *, uint_16 *, uint_8);

#endif /* PACKET_H_ */
