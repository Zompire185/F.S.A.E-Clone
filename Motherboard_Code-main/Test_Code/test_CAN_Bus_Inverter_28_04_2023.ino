/* Test per la comunicazione motherboard to inverter */
/* 28/04/2023 */
#include <bms_comunication.h>
#include <inverter_comunication.h>
#include <dashboard_comunication.h>

INVERTER_MSG Inverter_msg;   // classe per gli Inverter
LED_CHECK Led_Check;         // classe per i Led di stato
DASHBOARD Dashboard;         // classe per la Dashboard


// Definizione degli ID per la comunicazione CAN
uint32_t INVERTER_MSG::INV_1_ID = 0x01;
uint32_t INVERTER_MSG::INV_2_ID = 0x02;
uint32_t INVERTER_MSG::INV_3_ID = 0x05;
uint32_t INVERTER_MSG::INV_4_ID = 0x06;

// watchdog timer per gestire eventuali fault
elapsedMillis wait;

// Variabili per lo start dei Led
bool _reset_led = false;
// Buffer contenente lo stato dei led 
bool Buffer_Led[16] = {0};

// Funzioni di reset per Interrupt
void LED_OK(void);

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

  delay(20000);
   
   Serial.println("Setup comunicazione CAN_Bus: INVERTER...");
    // Setup INVERTER_CANbus
    
    if(Error_Free)
    {
      if(Inverter_msg.begin(wait, &Error_Free, Dashboard.Error_Buffer, Buffer_Led))
      {
        Serial.println(F("Setup INVERTER: COMPLETATO"));
      }
        else Serial.println(F("Setup INVERTER: FALLITO"));
    }  
}

// Start del timer per il loop
unsigned long cycle_prec_time = millis();
unsigned long cycle_curr_time;

// Setup dei buffer_tx e buffer_rx
uint8_t buf_tx[] = {0x00, 0x04, 0x08, 0x00, 0x05, 0x01, 0x00, 0x00};
uint8_t buf_rx[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void loop() {
  // Memorizzazione del tempo corrente del loop
  cycle_curr_time = millis();

  // Verifica della condizione di tempo
  if (cycle_curr_time - cycle_prec_time >= 300)
  {
    // COMUNICAZIONE INVERTER //
    // ****************************//
      Serial.println(F("Inoltro il messaggio..."));
      Serial.print(F("INVERTER_ID: ")); 
      Serial.println(Inverter_msg.INV_1_ID, HEX);
      Inverter_msg.TX_message(INVERTER::INV_1, Inverter_msg.INV_1_ID, buf_tx);

      Serial.println(F("Messaggio in arrivo..."));
      if(Inverter_msg.RX_message(INVERTER::INV_1, wait, buf_rx))
      {
         Serial.println(F("Messaggio:"));
        for(int i = 0; i < 8; i++)
        {
          Serial.print(buf_rx[i], HEX);Serial.println(F(","));
        }
        if(!Buffer_Led[14])
        {
          Buffer_Led[14] = true;
        }  
      } else if(Buffer_Led[14])
        {
          Buffer_Led[14] = false;
        }  
     
      // Reset del buffer di lettura
      for(int i = 0; i<8; i++)
      {
        buf_rx[i] = 0;
        } 
      
      Serial.println(F("Inoltro il messaggio..."));
      Serial.print(F("INVERTER_ID: ")); 
      Serial.println(Inverter_msg.INV_2_ID, HEX);
      Inverter_msg.TX_message(INVERTER::INV_2, Inverter_msg.INV_2_ID, buf_tx);

      Serial.println(F("Messaggio in arrivo..."));
      if(Inverter_msg.RX_message(INVERTER::INV_2, wait, buf_rx))
      {
        Serial.println(F("Messaggio:"));
        for(int i = 0; i < 8; i++)
        {
         Serial.print(buf_rx[i], HEX);Serial.println(F(","));
        }
        if(!Buffer_Led[14])
        {
          Buffer_Led[14] = true;
        }  
      } else if(Buffer_Led[14])
        {
          Buffer_Led[14] = false;
        }  
      
    //****************************//

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
//************************//
