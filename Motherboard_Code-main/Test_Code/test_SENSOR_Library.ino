/*
* Test di lettura dati dai sensori
*/

#include <sensor_lib.h>
#include <led_check.h>

void blink(void);


LED_CHECK led_check; // classe per i Led
SENSOR Sensor;		 // classe per i Sensori

// Funzioni di reset per Interrupt
void LED_OK(void);
void SENS_OK(void);

// Variabili per lo start dei Led e Sensori
 bool _reset_sensor = false; 
 bool _reset_led = false;

void setup()
{

	// RESET LED //
	attachInterrupt(digitalPinToInterrupt(LED_SET), LED_OK, RISING);
	led_check.led_begin(_reset_led);
	// disattivazione Interrupt_Led
	detachInterrupt(digitalPinToInterrupt(LED_SET));
	// Reset dei led di stato
	led_check.led_reset();
	
	// RESET SENSORI //
	attachInterrupt(digitalPinToInterrupt(SENS_SET), SENS_OK, RISING);
	Sensor.Sensor_begin(_reset_sensor);
	// disattivazione Interrupt_Sensori
	detachInterrupt(digitalPinToInterrupt(SENS_SET));

	pinMode(LED_BUILTIN, OUTPUT);
	blink();

}

// Start del timer per il loop
unsigned long cycle_prec_time = millis();
unsigned long cycle_curr_time;
// Start del timer per i sensori
unsigned long sens_prec_time = millis();
unsigned long sens_curr_time;

void loop()
{
	// Memorizzazione del tempo corrente del loop
	cycle_curr_time = millis();
	// Memorizzazione del tempo corrente dei sensori
	sens_curr_time = millis();

	// Verifica della condizione di tempo
	if (cycle_curr_time - cycle_prec_time >= 300)
	{
		if(sens_curr_time - sens_prec_time >= 1000)
		{	// LETTURA e ANALISI DATI DAI SENSORI //

			// Ricezione del valore dal sensore AIR_TEMP_1
			Sensor.Sensor_Data.air_temp_1 = Sensor.Get_Data(SENSOR_TYPE::AIR_TEMP_1);
			// Analisi del sensore AIR_TEMP_1
			Sensor.Check_Sensor(SENSOR_TYPE::AIR_TEMP_1, Sensor.Sensor_Data.air_temp_1);

			blink();
			delay(50);
			blink();

			sens_prec_time = sens_curr_time;
		}

		// set dei led di stato
		led_check.send_led_check(led_check.led_buf);

		blink();

		cycle_prec_time = cycle_curr_time;
	}
}


void blink(void)
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(100);
	digitalWrite(LED_BUILTIN, LOW);
}

// FUNZIONI PER INTERRUPT //
void LED_OK(void)
{
	_reset_led = true;
}

void SENS_OK(void)
{
	_reset_sensor = true;
}
