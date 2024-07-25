#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <SD.h>

class DataLogger 
	{
		public:
    			static void initialize();
    			static void logData(float x, float y);
		private:
    			static File dataFile;
    			static String generateFileName();
	};

#endif
