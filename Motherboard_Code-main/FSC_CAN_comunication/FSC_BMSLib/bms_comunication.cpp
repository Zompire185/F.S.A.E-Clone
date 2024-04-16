/* File delle funzioni per il protocollo CAN, 
 per la comunicazione al BMS
 *******************************************
 Author: Cirelli Leonardo
 Date: 31/03/2023
 Version: 1.0
*/


// Inclusione delle librerie da utilizzare
#include <bms_comunication.h>


/* definizione della porta CAN da utilizzare per comunicare al BMS
   nel nostro caso la porta settata: CAN 3
*/ 
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> mycan_3;

/* DEFINIZIONE DEI METODI PER LA COMUNICAZIONE CAN_BUS */

/// @brief Metodo di inizializzazione del canale CAN del BMS
/// @param watchdog timer di watchdog per la comunicazione
/// @param Error_Free variabile di check per gli errori
/// @param Error_Dashboard buffer degli errori per la dashboard
/// @param Buffer_Led buffer dei led di stato
bool BMS_MSG::begin(elapsedMillis watchdog, bool *Error_Free, uint8_t *Error_Dashboard, bool *Buffer_Led)
{

	// definizione della lunghezza del messaggio
	tx_msg_bms.len =  8;
	rx_msg_bms.len =  8;

	// flush del buffer in trasmissione
	for(int i = 0; i < tx_msg_bms.len; i++)
	{tx_msg_bms.buf[tx_msg_bms.len] = 0;}
	// flush del buffer in ricezione
	for(int i = 0; i < rx_msg_bms.len; i++)
	{rx_msg_bms.buf[rx_msg_bms.len] = 0;}

	//setup del ID_BMS
	tx_msg_bms.id = BMS_ID;
		
	// aggiungere eventualmente filtri BMS //
		
    mycan_3.begin();
	mycan_3.setBaudRate(500000);

	// Check della comunicazione
	uint8_t check_buf[8] = {0x40};
	BMS_MSG::Send_Msg(check_buf);
	// *********************** //
	while(watchdog < 500)
	{
		if(mycan_3.read(rx_msg_bms))
		{
			Buffer_Led[9] = true;	// Led_BMS ON
			Buffer_Led[14] = true;	// Led_CAN ON
			*Error_Free = true;
			return true;
		}	
	}
	// ERRORE CRITICO
	BMS_MSG::Send_Critical_Error(Buffer_Led);
	*Error_Free = false;
	Buffer_Led[9] = false;	// Led_BMS OFF
	Buffer_Led[14] = false;	// Led_CAN OFF
	return false;
} 
	


/// @brief Metodo di inoltro di un messaggio tramite CAN_BUS
/// @param write_buff buffer di scrittura del messaggio 
void BMS_MSG::Send_Msg(uint8_t* write_buff)
{
	for(int i = 0; i < tx_msg_bms.len; i++)
	{
		tx_msg_bms.buf[i] = write_buff[i];
	}
	mycan_3.write(tx_msg_bms); // canale CAN 3
    delay(10);
}


/// @brief Metodo di ricezione di un messaggio CAN
/// @param wait timer di watchdog 
/// @param read_buff buffer per la lettura del messaggio
/// @param Buffer_Led buffer dei led di stato
bool BMS_MSG::Receive_Msg(elapsedMillis wait, uint8_t* read_buff, bool* Buffer_Led)
{
   wait = 0;
   while(wait < 500)
   {
		if(mycan_3.read(rx_msg_bms)) // ritorna true o false
		{	
			for(int i = 0; i < rx_msg_bms.len; i++)
			{
				read_buff[i] = rx_msg_bms.buf[i];
			}
			return true;
		} 		
	}
	   // ERRORE CRITICO
	   BMS_MSG::Send_Critical_Error(Buffer_Led);
	   Buffer_Led[9] = false; // Led_BMS OFF
	   Buffer_Led[14] = false; // Led_CAN OFF
	   return false;
   
}


 /// @brief  Metodo di attivazione dell'errore critico
 /// @param Buffer_Led buffer dei led di stato
 void BMS_MSG::Send_Critical_Error(bool *Buffer_Led)
 {
	// ERRORE CRITICO
	   BMS_MSG::Send_Msg(SHUTDOWN);
	   if(!Buffer_Led[5])
	   {
		Buffer_Led[5] = true; // Led_SHUTDOWN ON
	   }
 }
	

