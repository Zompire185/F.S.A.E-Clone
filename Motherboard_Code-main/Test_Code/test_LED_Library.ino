#include <led_check.h>

LED_CHECK Led_Check; // classe per i Led

// Variabili per lo start dei Led
bool _reset_led = false;


 // Funzioni di reset per Interrupt
void LED_OK(void);

// Stampa nome led
void Stampa_Led(int i);

bool Led_Buffer[16];


void setup() 
{

  Serial.begin(115200);
  Serial.println(F("Reset Led..."));
  // RESET LED //
  attachInterrupt(digitalPinToInterrupt(LED_SET), LED_OK, RISING);
  if(Led_Check.led_begin(&_reset_led))
  {
    // Reset dei led di stato
    Led_Check.led_reset();
    Serial.println(F("Reset COMPLETATO"));
  } else 
  {
     Serial.println(F("Reset FALLITO"));
  }
  // disattivazione Interrupt_Led
  detachInterrupt(digitalPinToInterrupt(LED_SET));

  
}
  

// Start del timer per il loop
  unsigned long prec_time = millis();
  unsigned long curr_time;

  int num = 0;
    
void loop() 
{

  // Memorizzazione del tempo corrente
  curr_time = millis();

  if(curr_time - prec_time >= 4000)
  { 
     Serial.println(F("Inizio Stampa Led"));
     for(int i = 0; i < 16; i++)
     {
       Serial.println(i);
      Led_Buffer[i] = true;
      Led_Check.send_led_check(Led_Buffer);
      delay(200);
      Led_Buffer[i] = false;
      Led_Check.send_led_check(Led_Buffer);
     }
     Serial.println(F("Fine Stampa Led"));
     prec_time = curr_time;
  }
}

// FUNZIONI PER INTERRUPT //
void LED_OK(void)
{
  _reset_led = true;
}


void Stampa_Led(int i)
{
  if(i == 0)
  {
    Serial.println(F("LED: CAN_OK"));
  }
  else if(i == 1)
       {
          Serial.println(F("LED: FAN"));
       }
       else if(i == 2)
            {
              Serial.println(F("LED: CAN_OK"));
            }
            else if(i == 3)
                 {
                    Serial.println(F("LED: SENS_OK"));
                 }
                 else if(i == 4)
                      {
                        Serial.println(F("LED: DRSIG_OK"));
                      }
                      else if(i == 5)
                           {
                              Serial.println(F("LED: TELE_OK"));
                           }
                           else if(i == 6)
                                {
                                   Serial.println(F("LED: BMS_OK"));
                                }
                                else if(i == 7)
                                     {
                                        Serial.println(F("LED: BAT_LOW"));
                                     }
                                     else if(i == 8)
                                          {
                                             Serial.println(F("LED: COOL_OK"));
                                          }
                                          else if(i == 9)
                                               {
                                                  Serial.println(F("LED: OVERHEAT"));
                                               }
                                               else if(i == 10)
                                                    {
                                                      Serial.println(F("LED: SHUTDOWN"));
                                                    }
                                                    else if(i == 11)
                                                          {
                                                            Serial.println(F("LED: CHARGE"));
                                                          }
                                                          else if(i == 12)
                                                               {
                                                                  Serial.println(F("LED: I2C_OK"));
                                                               }
                                                               else if(i == 13)
                                                                    {
                                                                      Serial.println(F("LED: PUMP"));
                                                                    }
                                                                    else if(i == 14)
                                                                         {
                                                                           Serial.println(F("LED: BRAKE"));
                                                                         }
                                                                         else if(i == 15)
                                                                              {
                                                                                Serial.println(F("LED: RTD"));
                                                                              }
                        
}
