/* File header per la configurazione della comunicazione 
*  con la dashboard
**************************************
Author: Cirelli Leonardo
Date: 08/04/2023
Version: 1.0
*/

#ifndef DASHBOARD_H
#define DASHBOARD_H

// Librerie Utili

// Libreria per la comunicazione I2C
#include <Wire.h>
// Libreria gestione dei Led
#include <led_check.h>


//#define DASHBOARD_DATA 17
//#define DASHBOARD_CLK 16



// ***********************************************************************
// PACCHETTO DATI //
// 8-byte:
// 1' byte | 2' byte | 3' byte | 4' byte | 5' byte | 6' byte | 7' byte |
// COPPIA  CARICA_BATT     /        /          /        /         /     
// ***********************************************************************


// ****************************************************************************
// PACCHETTO ERRORE //
// 4-byte:
/* 1°-Byte
* 7' bit -> INVERTER 1
* 6' bit -> INVERTER_2
* 5' bit -> INVERTER_3
* 4' bit -> INVERTER_4
* 3' bit -> BMS
* 2' bit -> AIR_TEMP_1
* 1' bit -> AIR_TEMP_2
* 0' bit -> AIR_TEMP_3

 * 2°-byte
* 7' bit -> TIRE_SENS_1
* 6' bit -> TIRE_SENS_2
* 5' bit -> TIRE_SENS_3
* 4' bit -> TIRE_SENS_4
* 3' bit -> SUSP_SENS_1
* 2' bit -> SUSP_SENS_2
* 1' bit -> SUSP_SENS_3
* 0' bit -> SUSP_SENS_4

 * 3°-byte
* 7' bit -> DIR_CURR
* 6' bit -> INV_CURR
* 5' bit -> BRAKE_PRESS	
* 4' bit -> WAT_TEMP
* 3' bit -> WAT_PRESS
* 2' bit -> STEERING_SENS
* 1' bit -> COOLING_SENS
* 0' bit -> SENSOR

 * 4°-byte
* 7' bit -> MOTHERBOARD_TEMP
* 6' bit -> --
* 5' bit -> --
* 4' bit -> --
* 3' bit -> --
* 2' bit -> --
* 1' bit -> --
* 0' bit -> --
*/


//*****************************************************************************
class DASHBOARD 
{

    public:

    // metodo di inizializzazione della dashboard
    bool begin(void);

    // Metodo per la trasmissione dati alla dashboard
    void Send_Data(uint8_t *data_buffer);

    // Metodo per la ricezione dati dalla dashboard
    void Get_Data(uint8_t *buf_rx);

    // Metodo per l'invio degli errori
    void Send_Error(void);
	
	// Buffer di gestione degli errori
	uint8_t Error_Buffer[4];

    private:

    // Address della dashboard
    uint8_t DASHBOARD_ADDRESS = 0x40;

    // Buffer di scrittura alla dashboard
    uint8_t Data_Buffer_Tx[7];
    uint8_t Data_Buffer_Rx[8];

    
};
#endif