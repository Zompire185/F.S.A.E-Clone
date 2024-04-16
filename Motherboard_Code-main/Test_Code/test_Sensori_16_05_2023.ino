/* Test per la comunicazione sensori */
/* 16/05/2023 */
#include <bms_comunication.h>
#include <inverter_comunication.h>
#include <dashboard_comunication.h>
#include <sensor_lib.h>

LED_CHECK Led_Check;         // obj per i Led di stato
SENSOR Sensor;               // obj per i Sensori
DASHBOARD Dashboard;         // obj per la Dashboard
SENSOR_ID Sensor_ID;

// Variabili per lo start dei Led
bool _reset_led = false;
// Variabili per lo start dei Sensori
bool _reset_sensor = false;

// Buffer contenente lo stato dei led 
bool Buffer_Led[16] = {0};

// Funzioni di reset per Interrupt
void LED_OK(void);
void SENSOR_OK(void);

// Variabile che indica lo stato di errore
bool Error_Free = true;

void setup() {

  Serial.begin(115200);
  Serial.println(F("Reset dei LED..."));

  // RESET LED //
  attachInterrupt(digitalPinToInterrupt(LED_SET), LED_OK, RISING);
  if(Led_Check.led_begin(&_reset_led))
  {
    // Reset dei led di stato
    Led_Check.led_reset();
    Serial.println(F("Reset COMPLETATO"));
  } else Serial.println(F("Reset FALLITO")); 
  // disattivazione Interrupt_Led
  detachInterrupt(digitalPinToInterrupt(LED_SET));

  // RESET SENSORI
  Serial.println("Setup comunicazione Sensori...");
  attachInterrupt(digitalPinToInterrupt(SENS_SET), SENSOR_OK, RISING);
    if(Error_Free)
    {
      if(Sensor.Sensor_begin(&_reset_sensor, Dashboard.Error_Buffer, &Error_Free, Buffer_Led))
      {
        Serial.println(F("Setup SENSORI: COMPLETATO"));
      }
      else Serial.println(F("Setup SENSORI: FALLITO"));
    }
   // disattivazione Interrupt_Sensor
   detachInterrupt(digitalPinToInterrupt(SENS_SET));

   // set dei led di stato
    Led_Check.send_led_check(Buffer_Led);
  
}

// Start del timer per il loop
unsigned long cycle_prec_time = millis();
unsigned long cycle_curr_time;

void loop() {
  // Memorizzazione del tempo corrente del loop
  cycle_curr_time = millis();

  // Verifica della condizione di tempo
  if (cycle_curr_time - cycle_prec_time >= 300)
  {
    // COMUNICAZIONE SENSORI //
    // ****************************//  
    Serial.println(F("Prelevo i dati dal sensore TIRE_SENS_1"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.TIRE_SENS_1_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        

    Serial.println(F("Prelevo i dati dal sensore TIRE_SENS_2"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.TIRE_SENS_2_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        

    Serial.println(F("Prelevo i dati dal sensore TIRE_SENS_3"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.TIRE_SENS_3_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        

    Serial.println(F("Prelevo i dati dal sensore TIRE_SENS_4"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.TIRE_SENS_4_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        

    Serial.println(F("Prelevo i dati dal sensore WAT_TEMP"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.WAT_TEMP_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        
    Serial.println(F("Prelevo i dati dal sensore AIR_TEMP_1"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.AIR_TEMP_1_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        

    Serial.println(F("Prelevo i dati dal sensore AIR_TEMP_2"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.AIR_TEMP_2_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        

    Serial.println(F("Prelevo i dati dal sensore AIR_TEMP_3"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.AIR_TEMP_3_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        
    Serial.println(F("Prelevo i dati dal sensore WAT_PRESS"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.WAT_PRESS_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        // Faccio il check sui sensori
        
    Serial.println(F("Prelevo i dati dal sensore STEERING_SENS"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.STEERING_SENS_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);

    Serial.println(F("Prelevo i dati dal sensore STEERING_SENS"));
        // Prelevo i dati dal sensori
        Sensor.Tx_Addr(Sensor_ID.STEERING_SENS_ID);
    Serial.print(F("Analog: "));Serial.println(analogRead(A4), DEC);
        
                
        
    Sensor.Motherboard_Temp(Buffer_Led, Dashboard.Error_Buffer);
    Serial.print(F("Temperatura Motherboard:"));Serial.println(Sensor.Sensor_Data.Temperature);
    Serial.print(F("Umidità Motherboard:"));Serial.println(Sensor.Sensor_Data.Humidity);

    // set dei led di stato
    Led_Check.send_led_check(Buffer_Led);

    cycle_prec_time = cycle_curr_time;
  }
}


// FUNZIONI PER INTERRUPT //
void LED_OK(void)
{
  _reset_led = true;
}

void SENSOR_OK(void)
{
  _reset_sensor = true;
}
//************************//
