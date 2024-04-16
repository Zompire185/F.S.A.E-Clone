

/* main file per la scrittura del programma per la Teensy.
-------------------------
 Author: Cirelli Leonardo
 Date: 28/04/2023
 Version: 1.0
*/

/* Inclusione delle librerie di riferimento
 * (al loro interno le librerie hanno le dipendenze utili)*/

#include <bms_comunication.h>
#include <critical_error.h>
#include <inverter_comunication.h>
#include <sensor_lib.h>
#include <dashboard_comunication.h>
// #include <FSC_Telemetry.h>



// OGGETTI DI CLASSE //
INVERTER_MSG Inverter_msg;	 // classe per gli Inverter
BMS_MSG Bms_msg; // classe per il BMS
LED_CHECK led_check; // classe per i Led
SENSOR Sensor;		 // classe per i Sensori
DASHBOARD Dashboard; // classe per la Dashboard
//CRITICAL_ERROR Critical_Error; // classe per gli Errori

// VARIABILI GLOBALI //
// Definizione degli ID per la comunicazione CAN
uint32_t INVERTER_MSG::INV_1_ID = 0x01;
uint32_t INVERTER_MSG::INV_2_ID = 0x02;
uint32_t INVERTER_MSG::INV_3_ID = 0x05;
uint32_t INVERTER_MSG::INV_4_ID = 0x06;
uint32_t BMS_MSG::BMS_ID = 0x4D;

// watchdog timer per gestire eventuali fault
elapsedMillis wait;

// Variabile di controllo per accensione del veicolo
bool START = false;

// Variabile check dei sistemi della vettura
bool READY_TO_DRIVE = false;

// Variabili per lo start dei Led e Sensori
 bool _reset_sensor = false; 
 bool _reset_led = false;


// Funzioni di reset per Interrupt
void LED_OK(void);
void SENS_OK(void);

/* Funzione di Setup */
void setup()
{
  // Inizializzazione variabile di controllo errore critico
  Bms_msg.Error_Free = true;

  // Setup DASHBOARD (IMPORTANTE: la Dashboard deve essere la prima ad essere inizializzata)
  //Dashboard.begin();

  // Setup TELEMETRIA
	
	// RESET LED //
	attachInterrupt(digitalPinToInterrupt(LED_SET), LED_OK, RISING);
	 if(led_check.led_begin(&_reset_led))
   {
    // Reset dei led di stato
    led_check.led_reset();
  }
  else // Invio errore ai BOX
	// disattivazione Interrupt_Led
	detachInterrupt(digitalPinToInterrupt(LED_SET));

	// RESET SENSORI //
	attachInterrupt(digitalPinToInterrupt(SENS_SET), SENS_OK, RISING);
	if(!Sensor.Sensor_begin(&_reset_sensor))
	{
    // Invio Errore ai BOX e alla Dashboard
	}
	// disattivazione Interrupt_Sensori
	detachInterrupt(digitalPinToInterrupt(SENS_SET));
	
	// Setup BMS_CANbus (IMPORTANTE: il BMS va inizializzato prima degli Inverter)
	Bms_msg.begin();
	
	if(Bms_msg.Error_Free)
	{
		// Setup INVERTER_CANbus
		Inverter_msg.begin(wait);
	}

  // set dei led di stato
    led_check.send_led_check(led_check.led_buf);

	// Loop per l'accensione della vettura
  if(READY_TO_DRIVE)
  {
    led_check.led_buf[0] = true;
    /*while (!START)
    {
      // ATTESA DELL'ACCENSIONE DELLA VETTURA //

      // richiedere un valore booleano OK dalla dashboard //
      // if(OK)
      // START = true;
    }*/
  }
}

// Start del timer per il loop
unsigned long cycle_prec_time = millis();
unsigned long cycle_curr_time;
// Start del timer per i sensori
unsigned long sens_prec_time = millis();
unsigned long sens_curr_time;


/* Loop di esecuzione*/
void loop()
{
	// Memorizzazione del tempo corrente del loop
	cycle_curr_time = millis();
	// Memorizzazione del tempo corrente dei sensori
	sens_curr_time = millis();

	// Verifica della condizione di tempo
	if (cycle_curr_time - cycle_prec_time >= 300)
	{

		// COMUNICAZIONE INVERTER //
		// ****************************//
		uint8_t buf_inv[] = {0x00, 0x04, 0x08, 0x00, 0x05, 0x01, 0x00, 0x00};

			Inverter_msg.TX_message(INVERTER::INV_1, Inverter_msg.INV_1_ID, buf_inv);
			Inverter_msg.RX_message(INVERTER::INV_1, wait, buf_inv);
		//****************************//



		if(sens_curr_time - sens_prec_time >= 1000)
		{	// LETTURA e ANALISI DATI DAI SENSORI //
		
			// Ricezione del valore dal sensore TIRE_SENS_1
			Sensor.Sensor_Data.tire_sens_1 = Sensor.Get_Data(SENSOR_TYPE::TIRE_SENS_1);
			// Analisi del sensore TIRE_SENS_1
			Sensor.Check_Sensor(SENSOR_TYPE::TIRE_SENS_1, Sensor.Sensor_Data.tire_sens_1);

			// Ricezione del valore dal sensore TIRE_SENS_2
			Sensor.Sensor_Data.tire_sens_2 = Sensor.Get_Data(SENSOR_TYPE::TIRE_SENS_2);
			// Analisi del sensore TIRE_SENS_2
			Sensor.Check_Sensor(SENSOR_TYPE::TIRE_SENS_2, Sensor.Sensor_Data.tire_sens_2);

			// Ricezione del valore dal sensore TIRE_SENS_3
			Sensor.Sensor_Data.tire_sens_3 = Sensor.Get_Data(SENSOR_TYPE::TIRE_SENS_3);
			// Analisi del sensore TIRE_SENS_3
			Sensor.Check_Sensor(SENSOR_TYPE::TIRE_SENS_3, Sensor.Sensor_Data.tire_sens_3);

			// Ricezione del valore dal sensore TIRE_SENS_4
			Sensor.Sensor_Data.tire_sens_4 = Sensor.Get_Data(SENSOR_TYPE::TIRE_SENS_4);
			// Analisi del sensore TIRE_SENS_4
			Sensor.Check_Sensor(SENSOR_TYPE::TIRE_SENS_4, Sensor.Sensor_Data.tire_sens_4);

			// Ricezione del valore dal sensore AIR_TEMP_1
			Sensor.Sensor_Data.air_temp_1 = Sensor.Get_Data(SENSOR_TYPE::AIR_TEMP_1);
			// Analisi del sensore AIR_TEMP_1
			Sensor.Check_Sensor(SENSOR_TYPE::AIR_TEMP_1, Sensor.Sensor_Data.air_temp_1);

			// Ricezione del valore dal sensore AIR_TEMP_2
			Sensor.Sensor_Data.air_temp_2 = Sensor.Get_Data(SENSOR_TYPE::AIR_TEMP_2);
			// Analisi del sensore AIR_TEMP_2
			Sensor.Check_Sensor(SENSOR_TYPE::AIR_TEMP_2, Sensor.Sensor_Data.air_temp_2);

			// Ricezione del valore dal sensore AIR_TEMP_3
			Sensor.Sensor_Data.air_temp_3 = Sensor.Get_Data(SENSOR_TYPE::AIR_TEMP_3);
			// Analisi del sensore AIR_TEMP_3
			Sensor.Check_Sensor(SENSOR_TYPE::AIR_TEMP_3, Sensor.Sensor_Data.air_temp_3);

			// Ricezione del valore dal sensore WAT_TEMP
			Sensor.Sensor_Data.water_temp = Sensor.Get_Data(SENSOR_TYPE::WAT_TEMP);
			// Analisi del sensore WAT_TEMP
			Sensor.Check_Sensor(SENSOR_TYPE::WAT_TEMP, Sensor.Sensor_Data.water_temp);

			// Ricezione del valore dal sensore WAT_PRESS
			Sensor.Sensor_Data.water_press = Sensor.Get_Data(SENSOR_TYPE::WAT_PRESS);
			// Analisi del sensore WAT_PRESS
			Sensor.Check_Sensor(SENSOR_TYPE::WAT_PRESS, Sensor.Sensor_Data.water_press);

			// Ricezione del valore dal sensore STEERING_SENS
			Sensor.Sensor_Data.steering_data = Sensor.Get_Data(SENSOR_TYPE::STEERING_SENS);
			// Analisi del sensore STEERING_SENS
			Sensor.Check_Sensor(SENSOR_TYPE::STEERING_SENS, Sensor.Sensor_Data.steering_data);

			// Ricezione del valore dal sensore COOLING_SENS
			Sensor.Sensor_Data.cooling_data = Sensor.Get_Data(SENSOR_TYPE::COOLING_SENS);
			// Analisi del sensore COOLING_SENS
			Sensor.Check_Sensor(SENSOR_TYPE::COOLING_SENS, Sensor.Sensor_Data.cooling_data);

			// Ricezione del valore dal sensore SUSP_SENS_1
			Sensor.Sensor_Data.susp_data_1 = Sensor.Get_Data(SENSOR_TYPE::SUSP_SENS_1);
			// Analisi del sensore SUSP_SENS_1
			Sensor.Check_Sensor(SENSOR_TYPE::SUSP_SENS_1, Sensor.Sensor_Data.susp_data_1);

			// Ricezione del valore dal sensore SUSP_SENS_2
			Sensor.Sensor_Data.susp_data_2 = Sensor.Get_Data(SENSOR_TYPE::SUSP_SENS_2);
			// Analisi del sensore SUSP_SENS_1
			Sensor.Check_Sensor(SENSOR_TYPE::SUSP_SENS_2, Sensor.Sensor_Data.susp_data_2);

			// Ricezione del valore dal sensore SUSP_SENS_3
			Sensor.Sensor_Data.susp_data_3 = Sensor.Get_Data(SENSOR_TYPE::SUSP_SENS_3);
			// Analisi del sensore SUSP_SENS_1
			Sensor.Check_Sensor(SENSOR_TYPE::SUSP_SENS_3, Sensor.Sensor_Data.susp_data_3);

			// Ricezione del valore dal sensore SUSP_SENS_4
			Sensor.Sensor_Data.susp_data_4 = Sensor.Get_Data(SENSOR_TYPE::SUSP_SENS_4);
			// Analisi del sensore SUSP_SENS_1
			Sensor.Check_Sensor(SENSOR_TYPE::SUSP_SENS_4, Sensor.Sensor_Data.susp_data_4);

			// Leggo i valori dei temperatura e umidità
			Sensor.Motherboard_Temp();
			// ************************************************************************//
			sens_prec_time = sens_curr_time;
		}


		// INOLTRO DATI ALLA DASHBOARD //
		

		//****************************//

		// set dei led di stato
		led_check.send_led_check(led_check.led_buf);

		cycle_prec_time = cycle_curr_time;
	}
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
//************************//
