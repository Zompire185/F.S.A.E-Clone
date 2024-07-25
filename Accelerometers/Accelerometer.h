#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Wire.h>
#include <Adafruit_LIS3DH.h>

class Accelerometer 
	{
		public:
    			static void initialize();
    			static void readData(float &x, float &y);
	};

#endif
