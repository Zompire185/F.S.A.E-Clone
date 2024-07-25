#include "Transmission.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Transmission::canBus;

void Transmission::initialize() 
	{
    		canBus.begin();
    		canBus.setBaudRate(500000); // Set the baud rate to 500 kbps
	}

void Transmission::sendData(float x, float y) 
	{
    		CAN_message_t msg;
    		msg.id = 0x100; // CAN message ID
   		msg.len = 8;
    
       		memcpy(&msg.buf[0], &x, sizeof(float));
    		memcpy(&msg.buf[4], &y, sizeof(float));
    
    		canBus.write(msg);
	}
