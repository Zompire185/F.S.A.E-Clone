#include "DataLogger.h"
#include <TimeLib.h>

File DataLogger::dataFile;

void DataLogger::initialize() 
	{
    		String fileName = generateFileName();
    		dataFile = SD.open(fileName.c_str(), FILE_WRITE);
    		if (!dataFile) 
			{
        			Serial.println("Failed to create file");
    			}
	}

void DataLogger::logData(float x, float y) 
	{
    		if (dataFile) 
			{
        			String dataString = String(millis()) + "," + String(x) + "," + String(y);
        			dataFile.println(dataString);
        			dataFile.flush();
    			}
	}

String DataLogger::generateFileName() 
	{
    		return String("data_") + String(millis()) + String(".csv");
	}
