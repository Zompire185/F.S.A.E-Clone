/* File dei metodi per la configurazione della comunicazione 
*  con la dashboard
**************************************
Author: Cirelli Leonardo
Date: 08/04/2023
Version: 1.0
*/

#include <dashboard_comunication.h>

/// @brief Metodo di inizializzazione della comunicazione  
bool DASHBOARD::begin(void)
{
    // Inizializzazione della comunicazione I2C
    Wire.begin();

    // Inizializzazione dei buffer per il check dati
    uint8_t Dashboard_Check_Tx[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    uint8_t Dashboard_Check_Rx[8] = {0};

    // Inizializzazione dei buffer gli errori
    for(int i = 0; i < 4; i++)
    {Error_Buffer[i] = {0};}

    //Inizializzazione dei buffer di lettura e scrittura
    for(int i = 0; i < 7; i++)
    {Data_Buffer_Tx[i] = 0;}
    for(int i = 0; i < 8; i++)
	{Data_Buffer_Rx[i] = 0;}
    
	
	// Variabile di check
    bool DASHBOARD_OK = false;
	

    // Test della comunicazione 
    Wire.beginTransmission(DASHBOARD_ADDRESS);
        Wire.write(Dashboard_Check_Tx, 8);
        int i = 0;
        while(Wire.available() && i < 8)
        {
				Dashboard_Check_Rx[i] = Wire.read();
				i++;
        }
    Wire.endTransmission();
    
    // Check della correttezza dei dati
    for(i=0; i < 7; i++)
    {
        if(Dashboard_Check_Rx[i] == Dashboard_Check_Tx[i])
        {
            DASHBOARD_OK = true; 
        }
        else DASHBOARD_OK = false; 
    }

    if(!DASHBOARD_OK)
    {
        // Invio errore alla telemetria
        return false;
    }
    else return true;
}


/// @brief Metodo di Invio dati alla dashboard
/// @param data_buffer Buffer contentente i dati da inviare
void DASHBOARD::Send_Data(uint8_t *data_buffer)
{
    for(int i=0; i<7; i++)
    {
        Data_Buffer_Tx[i] = data_buffer[i];
    }

    Wire.beginTransmission(DASHBOARD_ADDRESS);
        Wire.write(Data_Buffer_Tx, 7);
    Wire.endTransmission();
}


/// @brief Metodo di ricezione dei dati alla dashboard
/// @param  buf_rx Buffer contentente per la lettura dati
void DASHBOARD::Get_Data(uint8_t *buf_rx)
{
	int i = 0;
	
         Wire.beginTransmission(DASHBOARD_ADDRESS);
        while(Wire.available())
        {
            buf_rx[i] = Wire.read();
			 
            i++;
        }
    Wire.endTransmission();
}


/// @brief Metodo di Invio errori alla dashboard
void DASHBOARD::Send_Error(void)
{
    Wire.beginTransmission(DASHBOARD_ADDRESS);
        Wire.write(Error_Buffer, 3);
    Wire.endTransmission();
}
