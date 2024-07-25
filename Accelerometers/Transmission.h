#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <FlexCAN_T4.h>

class Transmission 
	{
		public:
    			static void initialize();
    			static void sendData(float x, float y);
		private:
   		 	static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> canBus;
	};

#endif
