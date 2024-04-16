/* File Header per la configurazione del check sui led
-------------------------
 Author: Cirelli Leonardo
 Date: 21/03/2023
 Version: 1.0
*/

#ifndef LED_CHECK_H
#define LED_CHECK_H

// Inclusione delle librerie da utilizzare:

/// Libreria Standard di Arduino
#include <Arduino.h>

/// Definizione dei pin per il CLK e DATA sui Led
#define DATA_LED 3
#define CLK_LED 2
#define LED_SET 33


/// Dichiarazione della classe LED_CHECK
class LED_CHECK
{

	public:
	
			/// Metodo di inizializzazione dei led di stato
			bool led_begin(bool *start_led);
			
			/// Metodo di Reset per i led di stato
			void led_reset(void);

			/// metodo di aggiornamwnto dei led di stato
			void send_led_check(bool* led_buffer);
			
	private:
	
	// Variabile di Controllo
	int count;
		

};

#endif
