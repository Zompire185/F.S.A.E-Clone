/* File della definizione dei metodi 
   per la lettura dei valori dai sensori
   e per la gestione del sistema APPS ()
--------------------------
 Author: Cirelli Leonardo
 Date: 21/03/2023
 Version: 1.0
*/

// Inclusione delle librerie da utilizzare:
#include <sensor_lib.h>

DHT dht(DHT_PIN, DHT_TYPE); // classe per il sensore di temperatura DHT

/// @brief Metodo di inizializzazione dei sensori e dei canali di comunicazione
/// @param start_sensor variabile di inizializzazione dei sensori attesa dall'interrupt
bool SENSOR::Sensor_begin(bool *start_sensor, uint8_t *Error_Dashboard, bool *Error_Free, bool* Buffer_Led)
{
	// Canali di comunicazione in scrittura
	pinMode(SENS_CLK, OUTPUT);
	pinMode(SENS_ADDR, OUTPUT);
	
	// Canali di comunicazione in lettura
	pinMode(SENS_READ, INPUT);
	pinMode(DHT_PIN, INPUT);
	pinMode(SENS_SET, INPUT);
	
	// Inizializzazione del sensore DHT
	dht.begin();
	
	digitalWrite(SENS_CLK, HIGH);
	delay(10);
	digitalWrite(SENS_ADDR, LOW);

	count = 0;
	
	while (!start_sensor && count < 16)
	{
		digitalWrite(SENS_CLK, LOW);
		delay(10);
		digitalWrite(SENS_CLK, HIGH);
		count++;
	}
	if (!start_sensor)
	{
		// Comunicazione Errore alla Dashboard (Setup_Sensor)
		Error_Dashboard[3] |= 0b00000001;
		*Error_Free = false;	
		Buffer_Led[13] = false; // SENS_OK = OFF
	} 
	else 
	{
		*Error_Free = true;
		// Reset Errore alla Dashboard (Setup_Sensor)
		Error_Dashboard[3] &= 0b11111110;
		Buffer_Led[13] = true; // SENS_OK = ON
	}
	
	return Error_Free;
}

/// @brief Metodo di lettura dei dati del sensore
/// @param sensor Sensore da cui prendere i dati
float SENSOR::Get_Data(SENSOR_TYPE sensor)
{
	// Valore di lettura del sensore
	float S_value = 0;
	
	if(sensor == SENSOR_TYPE::TIRE_SENS_1) // Sensore: ZTP-148SRC1
	{
		// Invio dell'ID al sensore per la lettura dei dati grezzi
		SENSOR::Tx_Addr(Sensor_ID.TIRE_SENS_1_ID);	
		
		// Equazione della retta -20°/100° campionato a 14 bit
		S_value = (analogRead(SENS_READ)*0.0073) - 20.0;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::TIRE_SENS_2) // --
	{
		SENSOR::Tx_Addr(Sensor_ID.TIRE_SENS_2_ID);
		
		S_value = (analogRead(SENS_READ)*0.0073) - 20.0;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::TIRE_SENS_3) // --
	{
		SENSOR::Tx_Addr(Sensor_ID.TIRE_SENS_3_ID);
		
		S_value = (analogRead(SENS_READ)*0.0073) - 20.0;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::TIRE_SENS_4)	// --
	{
		SENSOR::Tx_Addr(Sensor_ID.TIRE_SENS_4_ID);
		
		S_value = (analogRead(SENS_READ)*0.0073) - 20.0;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::WAT_TEMP) // Sensore: PANW103395-395 (Termistore AQ)
	{
		SENSOR::Tx_Addr(Sensor_ID.WAT_TEMP_ID);
		
		// Beta = 3950.0
		// R_t = 10000.0
		// T_o = 25° (298.15 K)
		// _comp = funzione di compensazione (da fare)
		S_value = (3950.0*298.15)/(3950.0 + 298.15*log(10000.0/((10000.0*analogRead(SENS_READ))/16383 - analogRead(SENS_READ)))); // - _comp;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::AIR_TEMP_1) // Sensore: 2152721603 (Termistore AR)
	{
		SENSOR::Tx_Addr(Sensor_ID.AIR_TEMP_1_ID);
		
		// Beta = 3892.0
		// R_t = 10000.0
		// T_o = 25° (298.15 K)
		// _comp = funzione di compensazione (da fare)
		S_value = (3892.0*298.15)/(3892.0 + 298.15*log(10000.0/((10000.0*analogRead(SENS_READ))/16383 - analogRead(SENS_READ)))); // - _comp;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::AIR_TEMP_2) // --
	{
		SENSOR::Tx_Addr(Sensor_ID.AIR_TEMP_2_ID);
		// Beta = 3892.0
		// R_t = 10000.0
		// T_o = 25° (298.15 K)
		// _comp = funzione di compensazione (da fare)
		S_value = (3892.0*298.15)/(3892.0 + 298.15*log(10000.0/((10000.0*analogRead(SENS_READ))/16383 - analogRead(SENS_READ)))); // - _comp;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::AIR_TEMP_3) // --
	{
		SENSOR::Tx_Addr(Sensor_ID.AIR_TEMP_3_ID);
		
		// Beta = 3892.0
		// R_t = 10000.0
		// T_o = 25° (298.15 K)
		// _comp = funzione di compensazione (da fare)
		S_value = (3892.0*298.15)/(3892.0 + 298.15*log(10000.0/((10000.0*analogRead(SENS_READ))/16383 - analogRead(SENS_READ)))); // - _comp;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::WAT_PRESS) // Sensore: 114991172
	{
		SENSOR::Tx_Addr(Sensor_ID.WAT_PRESS_ID); 
		
		// Flow_max = 25L/min
		// - dividere per 60 -> L/sec
		S_value = (analogRead(SENS_READ)*25.0)/16383;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::STEERING_SENS) // MANCA
	{
		SENSOR::Tx_Addr(Sensor_ID.STEERING_SENS_ID);
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::COOLING_SENS) // Sensore: MIPAN2XX100PSAAX
	{
		SENSOR::Tx_Addr(Sensor_ID.COOLING_SENS_ID); 
		
		// P_max = 100psi
		S_value = (analogRead(SENS_READ)*100.0)/16383;
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::SUSP_SENS_1) // Potenziometri Lineari
	{
		SENSOR::Tx_Addr(Sensor_ID.SUSP_SENS_1_ID);
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::SUSP_SENS_2) // Potenziometri Lineari
	{
		SENSOR::Tx_Addr(Sensor_ID.SUSP_SENS_2_ID);
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::SUSP_SENS_3) // Potenziometri Lineari
	{
		SENSOR::Tx_Addr(Sensor_ID.SUSP_SENS_3_ID);
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::SUSP_SENS_4) // Potenziometri Lineari (estrapolato da uno dei precedenti)
	{
		SENSOR::Tx_Addr(Sensor_ID.SUSP_SENS_4_ID);
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::DIR_CURR) // Sensore: L08P150D15
	{
		SENSOR::Tx_Addr(Sensor_ID.DIR_CURR_ID);
		
		// Current_max = 150A
		S_value = (analogRead(SENS_READ)*150.0)/16383;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::INV_CURR) // Sensore: L08P150D15
	{
		SENSOR::Tx_Addr(Sensor_ID.INV_CURR_ID);
		
		// Current_max = 150A
		S_value = (analogRead(SENS_READ)*150.0)/16383;
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else if(sensor == SENSOR_TYPE::BRAKE_PRESS) // Sensore: MIPAF1XX667PSAAX
	{
		SENSOR::Tx_Addr(Sensor_ID.BRAKE_PRESS_ID); 
		
		// P_max = 667psi
		S_value = (analogRead(SENS_READ)*667.0)/16383;
		
		// Uncomment per convertire psi -> bar
		// S_value = S_value * 0.0689475729; 
		
		// Ritorno del valore letto dal sensore scelto
		return S_value;
	}
	else return 0;
	
}

/// @brief Metodo di lettura della temperatura della scheda madre (DHT11)
/// @param Buffer_Led Buffer dei led di stato
/// @param Error_Buffer Buffer degli errori alla dashboard
void SENSOR::Motherboard_Temp(bool* Buffer_Led, uint8_t *Error_Buffer)
{
	// Lettura dei valori di Temperatura e Umidità ambientale
	Sensor_Data.Temperature = dht.readTemperature();
	Sensor_Data.Humidity = dht.readHumidity();
	
	// Check sulla corretteza della lettura
	if(isnan(Sensor_Data.Temperature) || isnan(Sensor_Data.Humidity))
	{
		Error_Buffer[4] |= 0b10000000; // Setup dell'errore alla Dashboard
		if(Buffer_Led[13])
		{Buffer_Led[13] = false;} // SENS_OK = OFF	
	} 
	else 
		{
			Error_Buffer[4] &= 0b01111111; // Reset dell'errore alla Dashboard
			if(!Buffer_Led[13])
			{Buffer_Led[13] = true;} // SENS_OK = ON
		}
}

/// @brief Metodo di invio degli indirizzi dei sensori
///@param sens_addr Buffer contenente gli indirizzi dei sensori
void SENSOR::Tx_Addr(bool* sens_addr)
{
	digitalWrite(SENS_CLK, HIGH);
	digitalWrite(SENS_ADDR, LOW);
	
	for(int i=7; i<0; i--)
		{	
			digitalWrite(SENS_CLK, LOW);

			if(sens_addr[i] == 0)
			digitalWrite(SENS_ADDR, LOW);
			else if (sens_addr[i] == 1)
			digitalWrite(SENS_ADDR, HIGH);

			digitalWrite(SENS_CLK, HIGH);
		}
}

/// Metodo per il check della correttezza dati dei sensori 
void SENSOR::Check_Sensor(SENSOR_TYPE sensor, float sens_value, uint8_t *Error_Dashboard, bool *Buffer_Led)
{
	if(sensor == SENSOR_TYPE::TIRE_SENS_1)
	{
		if(sens_value <= -25.0 && sens_value >= 120.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[2] |= 0b10000000; 
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else 
		{
			// Annullamento del bit di errore //
			Error_Dashboard[2] &= 0b01111111; 
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; 
		}
	}
	else if(sensor == SENSOR_TYPE::TIRE_SENS_2)
	{
		if(sens_value <= -25.0 && sens_value >= 120.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[2] |= 0b01000000; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else 
		{
			// Annullamento del bit di errore //
			Error_Dashboard[2] &= 0b10111111; 
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::TIRE_SENS_3)
	{
		if(sens_value <= -25.0 && sens_value >= 120.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[2] |= 0b00100000; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else 
		{
			// Annullamento del bit di errore //
			Error_Dashboard[2] &= 0b11011111; 
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::TIRE_SENS_4)
	{
		if(sens_value <= -25.0 && sens_value >= 120.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[2] |= 0b00010000; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else 
		{
			// Annullamento del bit di errore //
			Error_Dashboard[2] &= 0b11101111;
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::AIR_TEMP_1)
	{
		if(sens_value <= -42.0 && sens_value >= 130.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[1] |= 0b00000100; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup led COOL_OK
			Buffer_Led[7] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else 
		{
			// Annullamento del bit di errore //
			Error_Dashboard[1] &= 0b11111011; 
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup led COOL_OK
			Buffer_Led[7] = true;
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::AIR_TEMP_2)
	{
		if(sens_value <= -42.0 && sens_value >= 137.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[1] |= 0b00000010; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup led COOL_OK
			Buffer_Led[7] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else 
		{
			// Annullamento del bit di errore //
			Error_Dashboard[1] &= 0b11111101;
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup led COOL_OK
			Buffer_Led[7] = true;
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::AIR_TEMP_3)
	{
		if(sens_value <= -42.0 && sens_value >= 137.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[1] |= 0b00000001; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup led COOL_OK
			Buffer_Led[7] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else 
		{
			// Annullamento del bit di errore //
			Error_Dashboard[1] &= 0b11111110;
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup led COOL_OK
			Buffer_Led[7] = true;
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::SUSP_SENS_1)
	{
		// MANCANO DATI //
	}
	else if(sensor == SENSOR_TYPE::SUSP_SENS_2)
	{
		// MANCANO DATI //
	}
	else if(sensor == SENSOR_TYPE::SUSP_SENS_3)
	{
		// MANCANO DATI //
	}
	else if(sensor == SENSOR_TYPE::SUSP_SENS_4)
	{
		// MANCANO DATI //
	}
	else if(sensor == SENSOR_TYPE::DIR_CURR)
	{
		if(sens_value > 150.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[3] |= 0b10000000; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else
		{
			// Annullamento del bit di errore //
			Error_Dashboard[3] &= 0b01111111; 
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::INV_CURR)
	{
		if(sens_value > 150.0) 
		{	
			// Setup del bit di errore //
			Error_Dashboard[3] |= 0b01000000; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else
		{
			// Annullamento del bit di errore //
			Error_Dashboard[3] &= 0b10111111; 
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::WAT_TEMP)
	{
		if(sens_value <= -52.0 && sens_value >= 152.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[3] |= 0b00010000; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup led COOL_OK
			Buffer_Led[7] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else
		{
			// Annullamento del bit di errore //
			Error_Dashboard[3] &= 0b11101111; 
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup led COOL_OK
			Buffer_Led[7] = true;
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::BRAKE_PRESS)
	{
		if(sens_value >= 670.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[3] |= 0b00100000; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else
		{
			// Annullamento del bit di errore //
			Error_Dashboard[3] &= 0b11011111; 
			
			// Setup dei led
			Buffer_Led[1] = true; 
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::WAT_PRESS)
	{
		if(sens_value <= 0.5 && sens_value >= 27.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[3] |= 0b00001000; 
			
			// Setup dei led
			Buffer_Led[13] = false;
			// Setup led COOL_OK
			Buffer_Led[7] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else
		{
			// Annullamento del bit di errore //
			Error_Dashboard[3] &= 0b11110111; 
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup led COOL_OK
			Buffer_Led[7] = true;
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
	else if(sensor == SENSOR_TYPE::STEERING_SENS)
	{
		// DA FARE //
	}
	else if(sensor == SENSOR_TYPE::COOLING_SENS)
	{
		if(sens_value <= 0.0 && sens_value >= 102.0)
		{
			// Setup del bit di errore //
			Error_Dashboard[3] |= 0b00000010; 
			
			// Setup del led SENS_OK
			Buffer_Led[13] = false;
			// Setup led COOL_OK
			Buffer_Led[7] = false;
			// Setup del led OVERHEAT
			Buffer_Led[6] = true;  // ON se in ERROR
		}
		else
		{
			// Annullamento del bit di errore //
			Error_Dashboard[3] &= 0b11111101; 
			
			// Setup dei led
			Buffer_Led[13] = true; 
			// Setup led COOL_OK
			Buffer_Led[7] = true;
			// Setup del led OVERHEAT
			Buffer_Led[6] = false; // OFF se in NON-ERROR
		}
	}
}


/// Metodo di gestione APPS
void SENSOR::APPS_Manager(void)
{
	
	// Lettura dei dati
	Sensor_Data.brake_data = SENSOR::Get_Data(SENSOR_TYPE::BRAKE_PRESS);
	Sensor_Data.dir_curr_data = SENSOR::Get_Data(SENSOR_TYPE::DIR_CURR);
	
	// gestione dei due valori
	//	- se non rispettano le soglie -> - comunicare il guasto al display
	//									 - accendere i led corrispondenti
	// deve essere un errore critico
}
