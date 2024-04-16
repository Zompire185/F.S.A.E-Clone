/* File dei metodi di accensioni dei Led di stato
 
 Author: Cirelli Leonardo
 Date: 21/03/2023
 Version: 1.0
*/

// Inclusione delle librerie da utilizzare
#include <led_check.h>

	/// Metodo di inizializzazione dei led di stato
	bool LED_CHECK::led_begin(bool *start_led)
	{	
		pinMode(DATA_LED, OUTPUT);
		pinMode(CLK_LED, OUTPUT);
		pinMode(LED_SET, INPUT);
		
		count = 0;
		
		digitalWrite(CLK_LED, HIGH);
		delay(10);
		digitalWrite(DATA_LED, LOW);

		while (!(*start_led) && count < 16)
		{
		digitalWrite(CLK_LED, LOW);
		delay(10);
		digitalWrite(CLK_LED, HIGH);
		count++;
		}
		if (!(*start_led))
		{
			// Comunicazione Errore ai Led di Stato
			return false;
		}
		else return true;
	
	}
	
	/// Metodo di Reset per i led di stato
	void LED_CHECK::led_reset(void)
	{
		// RESET LED //
		digitalWrite(CLK_LED, HIGH);
		delay(10);
		digitalWrite(DATA_LED, LOW);
		
		for(int i=0; i<16; i++)
		{
				digitalWrite(CLK_LED, LOW);
				delay(10);
				digitalWrite(CLK_LED, HIGH);
		}
		
	}

	/// Metodo di scrittura del buffer per aggiornare lo stato dei led
	/// @param led_buf[] Buffer per l'attivazione dei led
	void LED_CHECK::send_led_check(bool* led_buffer)
	{

		digitalWrite(CLK_LED, HIGH);
		
		for(int i=0; i < 16; i++)
		{	
			digitalWrite(CLK_LED, LOW);
			
			if(!led_buffer[i])
				digitalWrite(DATA_LED, LOW);
			else if (led_buffer[i])
				digitalWrite(DATA_LED, HIGH);

			delay(50);
			
			digitalWrite(CLK_LED, HIGH);
			
		}
	}
	
