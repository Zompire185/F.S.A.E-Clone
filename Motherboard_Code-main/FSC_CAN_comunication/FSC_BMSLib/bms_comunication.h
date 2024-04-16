/* File header per la configurazione della comunicazione con il BMS
 Author: Cirelli Leonardo
 Date: 31/03/2023
 Version: 1.0
*/

/* Direttiva di pre-processore utile a non duplicare il codice nel sorgente*/
#ifndef BMS_COMUNICATION_H
#define BMS_COMUNICATION_H

// Inclusione delle librerie da utilizzare:

/// Libreria per il protocollo CAN (https://github.com/tonton81/FlexCAN_T4)
#include <FlexCAN_T4.h>
/// Libreria Standard di Arduino
#include <Arduino.h>


class BMS_MSG
{
    public:

    // Membri statici degli ID per Inverter
    static uint32_t	BMS_ID;

    // Metodo che permette di inizializzare le porte CAN della Teensy
	bool begin(elapsedMillis watchdog,bool *Error_Free, uint8_t* Error_Dashboard, bool* Buffer_Led);

    // Metodo che permette di inoltrare un messaggio
	void Send_Msg(uint8_t* write_buff);

	// Metodo che pemette di ricevere un messaggio tramite puntatore
	bool Receive_Msg(elapsedMillis wait, uint8_t* read_buff, bool* Buffer_Led);

    // Metodo di inoltro dell'errore critico
    void Send_Critical_Error(bool *Buffer_Led);
	
    private:
    
    // Definizione dell'oggetto messaggio per il BMS
    CAN_message_t tx_msg_bms;
    CAN_message_t rx_msg_bms;
    
    // Codice di errore critico
    uint8_t SHUTDOWN[8] = { 0xff };
};
#endif