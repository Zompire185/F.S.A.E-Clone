#include "Accelerometer.h"

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void Accelerometer::initialize() 
	{
    		if (!lis.begin(0x18)) 
			{
        			Serial.println("Could not start LIS3DH");
        			while (1);
    			}
    		lis.setRange(LIS3DH_RANGE_8_G); // 2, 4, 8 or 16 G!
	}

void Accelerometer::readData(float &x, float &y) 
	{
    		lis.read();
    		x = lis.x;
    		y = lis.y;
	}
