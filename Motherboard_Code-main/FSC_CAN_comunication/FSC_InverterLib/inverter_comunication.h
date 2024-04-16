/* File Header per la configurazione del protocollo CAN
 Author: Cirelli Leonardo
 Date: 31/03/2023
 Version: 1.0
*/




/* Direttiva di pre-processore utile a non duplicare il codice nel sorgente*/
#ifndef INV_COMUNICATION_H
#define INV_COMUNICATION_H
// Inclusione delle librerie da utilizzare:

/// Libreria per il protocollo CAN (https://github.com/tonton81/FlexCAN_T4)
#include <FlexCAN_T4.h>
/// Libreria Standard di Arduino
#include <Arduino.h>
/// Libreria comunicazione BMS
#include <bms_comunication.h>


/* Classe enumerativa per l'assegnazione della porta CAN */
enum class INVERTER
{
	INV_1 = 1, 
	INV_2,
	INV_3,
	INV_4,
};


/* Dichiarazione della classe INVERTER_MSG */
class INVERTER_MSG {
	
	/* Dichiarazione dei metodi per la comunicazione CAN_BUS */
	public:
	
		// Metodo che permette di inizializzare le porte CAN della Teensy
		bool begin(elapsedMillis watchdog, bool *Error_Free, uint8_t *Error_Dashboard, bool *Buffer_Led);

		// Metodo che permette di inoltrare un messaggio
		void TX_message(INVERTER port, uint8_t* write_buff);

		// Metodo che pemette di ricevere un messaggio tramite puntatore
		bool RX_message(INVERTER port, elapsedMillis wait, uint8_t* read_buff);

	private:
	
		// Membri statici degli ID per Inverter
		static const uint32_t INV_1_ID = 0x01;
		static const uint32_t	INV_2_ID = 0x02;
		static const uint32_t INV_3_ID = 0x05;
		static const uint32_t	INV_4_ID = 0x06;
		
		// Definizione dell'oggetto messaggio per l'Inverter
		CAN_message_t tx_msg_inverter;
		CAN_message_t rx_msg_inverter;

		// Oggetto di classe
		BMS_MSG inverter_critical; // obj per l'inoltro dell'errore critico
};
#endif
