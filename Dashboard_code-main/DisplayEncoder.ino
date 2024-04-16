#include <EncoderStepCounter.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
#include <TouchScreen.h>

#define POS1 50
#define POS2 120
#define POS3 190
#define WHITE   0xFFFF

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
int16_t BOXSIZE;
int16_t PENRADIUS = 30;
uint16_t ID, oldcolor, currentcolor;
uint8_t Orientation = 1;   

 
const int pin1 = 20;
const int pin2 = 21;
 

EncoderStepCounter encoder(pin1, pin2);
 

int oldPosition = 0;
 
const int buttonPin = 36;    // pushbutton pin
int lastButtonState = LOW;  // last button state
int debounceDelay = 5;      // debounce time for the button in ms
 int buttonState ;
 int value;
 int i;


 void pagina1(){
   delay(50);
   
   Serial.print("buttonstate");
   Serial.println(buttonState);
   //buttonState = digitalRead(buttonPin);
   tft.fillScreen(BLACK);
   value= analogRead(A10);
   value= map(value,0,1024,0,9);
   for(i=0;i<=value;i++){
     tft.fillScreen(BLACK);
     tft.fillRect(10, 150, 100, BOXSIZE, WHITE);
     delay(50);

   }
 while(buttonState== LOW){
   int value1= analogRead(A10);
   value1= map(value1,0,1024,0,9);
   Serial.println(value1);
    BOXSIZE = tft.width() / 10;

   if(value1 != value){
     for(i=0;i<value1;i++){
     
     if(value1<5){
       tft.fillRect(10, 210-(i*20), 100, 30, GREEN);}
       if(value1>=5){
       tft.fillRect(10, 210-(i*20), 100, 30, RED);}
     delay(50);

   }
   for(i=value1 ;value1<=9;value1++){
      tft.fillRect(10, 210-(i*20), 100, 30, BLACK);
     
   }
   
   }

 }



 }
 
 
 
 
 
 void schermo_1 (){
   tft.setCursor(1,POS1);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print("SCHERMO 1");  

    
    
    tft.setCursor(1,POS2);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 2"); 




    tft.setCursor(1,POS3);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 3");  
 }

 void schermo_2 (){
   tft.setCursor(1,POS1);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 1");  

    
    
    tft.setCursor(1,POS2);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print("SCHERMO 2"); 




    tft.setCursor(1,POS3);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 3");  
 }
 void schermo_3 (){
   tft.setCursor(1,POS1);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 1");  

    
    
    tft.setCursor(1,POS2);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 2"); 
    



    tft.setCursor(1,POS3);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    tft.print("SCHERMO 3");  
   
 }
 
void setup() {
  Serial.begin(9600);
  // Initialize encoder
  encoder.begin();
  // Initialize interrupts
  attachInterrupt(digitalPinToInterrupt(pin1), interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin2), interrupt, CHANGE);
  // set the button pin as an input_pullup:
  pinMode(buttonPin, INPUT_PULLUP);
  tft.reset();
   ID = tft.readID();
    tft.begin(ID);
    Serial.begin(9600);
    //show_Serial();
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
    BOXSIZE = tft.width() / 6;
    tft.fillScreen(BLACK);
     tft.setCursor(0, 0);
    tft.setTextSize(1);
    //tft.fillCircle(125,150, PENRADIUS, RED);
   // tft.fillRect(100, 150, BOXSIZE, BOXSIZE, WHITE);
    tft.setCursor(1,POS1);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 1");  

    
    
    tft.setCursor(1,POS2);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 2"); 




    tft.setCursor(1,POS3);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print("SCHERMO 3");  

    delay(1000);

   // show_tft();
}
 
void loop() {
  // if you're not using interrupts, you need this in the loop:
  encoder.tick();
 
  // read encoder position:
  int position = encoder.getPosition();
  // read the pushbutton:
  int buttonState = digitalRead(buttonPin);
  //  // if the button has changed:
  if (buttonState != lastButtonState) {
    // debounce the button:
    delay(debounceDelay);
    // if button is pressed:
    if (buttonState == LOW && abs(position)==1) {
      Serial.print("you pressed on position: ");
      Serial.println(position);
      pagina1();

      
    }
  }

  lastButtonState = buttonState;
 

  if (position % 4 == 0) {
    encoder.reset();
    position = encoder.getPosition();
  }
  
  if (position != oldPosition) {
    Serial.println(position);
    
    oldPosition = position;
  }
  if (position==1 || position==-1){
    schermo_1();
  }
  if (position==2 || position==-2){
    schermo_2();
   
  }
  if (position==3 || position==-3){
    schermo_3();
  }
}
 
// Call tick on every change interrupt
void interrupt() {
  encoder.tick();
}
