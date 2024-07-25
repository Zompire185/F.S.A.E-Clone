#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <IntervalTimer.h>
#include "Accelerometer.h"
#include "DataLogger.h"
#include "Transmission.h"

#define SD_CS_PIN 10

IntervalTimer timer;

void readAndLogData() 
	{
    		float x, y;
    		Accelerometer::readData(x, y);
    		DataLogger::logData(x, y);
    		Transmission::sendData(x, y);
	}

void setup() 
	{
    		Serial.begin(115200);
    		if (!SD.begin(SD_CS_PIN)) 
		{
        		Serial.println("SD card initialization failed!");
        		return;
    		}
   	 	Accelerometer::initialize();
    		DataLogger::initialize();
    		Transmission::initialize();

    		timer.begin(readAndLogData, 100000); // Leggi e salva i dati ogni 100ms
	}

void loop() 
{
    
}
