/* File delle funzioni per il protocollo CAN, 
 per la comunicazione con l'Inverter
 *******************************************
 Author: Cirelli Leonardo
 Date: 27/04/2023
 Version: 2.0
*/

// Inclusione delle librerie da utilizzare
#include <inverter_comunication.h>


/* definizione delle porte CAN da utilizzare per comunicare all'Inverter
   nel nostro caso le porte: CAN 1 e CAN 2
*/ 
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> mycan_1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> mycan_2;


	/* DEFINIZIONE DEI METODI PER LA COMUNICAZIONE CAN_BUS */

  //LED_CHECK led_check; // classe per i Led


	/*Funzione che permette di inizializzare le porte CAN 
	della Teensy. Questo metodo procede anche ad un check iniziale 
	sulla comunicazione con gli INVERTER, ma non sulla correttezza del dato.
	*/
/// @brief Metodo di inizializzazione del canali CAN
/// @param watchdog timer di watchdog
/// @param Error_Free variabile di check per gli errori
/// @param Error_Dashboard buffer degli errori per la dashboard
/// @param Buffer_Led buffer dei led di stato
bool INVERTER_MSG::begin(elapsedMillis watchdog, bool *Error_Free, uint8_t *Error_Dashboard, bool *Buffer_Led)
{
	// definizione della lunghezza del messaggio
	tx_msg_inverter.len = 8;
	rx_msg_inverter.len = 8;

	// Inizializzazione del buffer in ricezione
	for(int i = 0; i < rx_msg_inverter.len; i++)
	{rx_msg_inverter.buf[i] = 0;}

	// Inizializzazione del buffer in scrittura
	for(int i = 0; i < rx_msg_inverter.len; i++)
	{tx_msg_inverter.buf[i] = 0;}
	

	mycan_1.begin();
	mycan_1.setBaudRate(500000);

	mycan_2.begin();
	mycan_2.setBaudRate(500000);


	// Check degli stati di errore degli inverter (lettura dei registri di errore)
	// settare i bit di errore per la dashboard

	// Buffer di check 
	uint8_t tx_check_buffer[8] = {0}; 
	uint8_t rx_check_buffer[8] = {0}; 


		// Check correttezza dati scritti negli inverter
		// INVERTER 1
		if (Error_Free)
		{
		//Invio Messaggio
		INVERTER_MSG::TX_message(INVERTER::INV_1, tx_check_buffer);
		// Ricevo un messaggio 
			if (!INVERTER_MSG::RX_message(INVERTER::INV_1, watchdog, rx_check_buffer))
			{
				*Error_Free = false;
				inverter_critical.Send_Critical_Error(Buffer_Led);
				// Bit di errore Inverter_1 = 1
				Error_Dashboard[1] |= 0b10000000;
				if(Buffer_Led[14])
				{Buffer_Led[14] = false;} //CAN_OK = OFF
			}
			else 
			{
				// Bit di errore Inverter_1 = 0
				Error_Dashboard[1] &= 0b01111111;
				if(!Buffer_Led[14])
				{Buffer_Led[14] = true;} //CAN_OK = ON
			}
		}
		
		
		// INVERTER 2	
		if (Error_Free)
		{
			// Invio Messaggio
			INVERTER_MSG::TX_message(INVERTER::INV_2, tx_check_buffer);
		// Ricevo un messaggio 
			if (!INVERTER_MSG::RX_message(INVERTER::INV_2, watchdog, rx_check_buffer))
			{
				*Error_Free = false;
				inverter_critical.Send_Critical_Error(Buffer_Led);
				//Bit di errore Inverter_" = 1
				Error_Dashboard[1] |= 0b01000000;
				if(Buffer_Led[14])
				{Buffer_Led[14] = false;} //CAN_OK = OFF
			}
			else 
			{
				// Bit di errore Inverter_2 = 0
				Error_Dashboard[1] &= 0b10111111;
				if(!Buffer_Led[14])
				{Buffer_Led[14] = true;} //CAN_OK = ON
			}
		}
		

		// INVERTER 3
		if (Error_Free)
		{
			// Invio Messaggio
			INVERTER_MSG::TX_message(INVERTER::INV_3, tx_check_buffer);
		// Ricevo un messaggio 
			if (!INVERTER_MSG::RX_message(INVERTER::INV_3, watchdog, rx_check_buffer))
			{
				*Error_Free = false;
				inverter_critical.Send_Critical_Error(Buffer_Led);
				// Bit di errore Inverter_3 = 1
				Error_Dashboard[1] |= 0b00100000;
				if(Buffer_Led[14])
				{Buffer_Led[14] = false;} //CAN_OK = OFF

			}
			else 
			{
				// Bit di errore Inverter_3 = 0
		    	Error_Dashboard[1] &= 0b11011111;
				if(!Buffer_Led[14])
				{Buffer_Led[14] = true;} //CAN_OK = ON
			}
		}
		
		
		
		// INVERTER 4
		if (Error_Free)
		{
			// Invio Messaggio
			INVERTER_MSG::TX_message(INVERTER::INV_4, tx_check_buffer);
		// Ricevo un messaggio 
			if (!INVERTER_MSG::RX_message(INVERTER::INV_4, watchdog, rx_check_buffer))
			{
				*Error_Free = false;
				inverter_critical.Send_Critical_Error(Buffer_Led);
				// Bit di errore Inverter_4 = 1
				Error_Dashboard[1] |= 0b00010000;
				if(Buffer_Led[14])
				{Buffer_Led[14] = false;} //CAN_OK = OFF

			}
			else 
			{
				// Bit di errore Inverter_4 = 0
				Error_Dashboard[1] &= 0b11101111;
				if(!Buffer_Led[14])
				{Buffer_Led[14] = true;} //CAN_OK = ON
			}
		}
		
		// Check sui sensori riguardanti gli inverter //

		return Error_Free;
}
		

	/*
	Metodo che pemette di inoltrare il messaggio
	*/
	/// Metodo di inoltro di un messaggio tramite CAN_BUS
	/// @param port canale CAN scelto (INV)
	/// @param id_can Identificativo dell'Inverter con cui comunicare
	/// @param write_buff buffer di scrittura del messaggio 
	void INVERTER_MSG::TX_message(INVERTER port, uint8_t* write_buff)
	{
		if(port == INVERTER::INV_1)
		{
			//setup del buffer per l'inverter 1
				uint32_t id_can = INV_1_ID + 0x183;
				tx_msg_inverter.id = id_can;
				for(int i = 0; i < tx_msg_inverter.len; i++)
				{
					tx_msg_inverter.buf[i] = write_buff[i];
				}
				mycan_1.write(tx_msg_inverter); // canale CAN 1
				delay(10);
		}
		else if (port == INVERTER::INV_2)
		{
			//setup del buffer per l'inverter 2
				uint32_t id_can = INV_2_ID + 0x183;
				tx_msg_inverter.id = id_can;
				for(int i = 0; i < tx_msg_inverter.len; i++)
				{
					tx_msg_inverter.buf[i] = write_buff[i];
				}
				mycan_1.write(tx_msg_inverter); // canale CAN 1
				delay(10);
		}
		else if(port == INVERTER::INV_3)
		{
			//setup del buffer per l'inverter 3
				uint32_t id_can = INV_3_ID + 0x183;
				tx_msg_inverter.id = id_can;
				for(int i = 0; i < tx_msg_inverter.len; i++)
				{
					tx_msg_inverter.buf[i] = write_buff[i];
				}
				mycan_2.write(tx_msg_inverter); // canale CAN 2
				delay(10);
		}
		else if(port == INVERTER::INV_4)
		{
			//setup del buffer per l'inverter 4
				uint32_t id_can = INV_4_ID + 0x183;
				tx_msg_inverter.id = id_can;
				for(int i = 0; i < tx_msg_inverter.len; i++)
				{
					tx_msg_inverter.buf[i] = write_buff[i];
				}
				mycan_2.write(tx_msg_inverter); // canale CAN 2
				delay(10);
		}
		
	}
	
	
	/// Metodo di ricezione di un messaggio CAN
	/// (il metodo read() restituisce true o false)
	/// @param port canale CAN scelto (INV) o (BMS)
	/// @param wait ogetto per il watchdog
	/// @param read_buff buffer di lettura del messaggio 
	bool INVERTER_MSG::RX_message(INVERTER port, elapsedMillis wait, uint8_t* read_buff)
	{
		// Inizializzazione del watchdog
		wait = 0;
		
		if(port == INVERTER::INV_1)
		{
			while(wait < 500)
			{
				if(mycan_1.read(rx_msg_inverter)) 
				{	
					// riempio il buffer
					for(int i = 0; i < rx_msg_inverter.len; i++)
					{
						read_buff[i] = rx_msg_inverter.buf[i];
					}
					// exit
					return true;
				}
			}
		}
		else if (port == INVERTER::INV_2)
		{
			while(wait < 500)
			{
				if(mycan_1.read(rx_msg_inverter))
				{	
					// riempio il buffer
					for(int i = 0; i < rx_msg_inverter.len; i++)
					{
						read_buff[i] = rx_msg_inverter.buf[i];
					}
					// exit
					return true;
				}
			}	
		}
		else if(port == INVERTER::INV_3)
		{
			while(wait < 500)
			{
				if(mycan_2.read(rx_msg_inverter)) 
				{	
					// riempio il buffer
					for(int i = 0; i < rx_msg_inverter.len; i++)
					{
						read_buff[i] = rx_msg_inverter.buf[i];
					}
					// exit
					return true;
				}
			}			
		}
		else if(port == INVERTER::INV_4)
		{
			while(wait < 500)
			{
				if(mycan_2.read(rx_msg_inverter)) 
				{	
					// riempio il buffer
					for(int i = 0; i < rx_msg_inverter.len; i++)
					{
						read_buff[i] = rx_msg_inverter.buf[i];
					}
					// exit
					return true;
				}
			}	
		}						
		return false;
	}	


