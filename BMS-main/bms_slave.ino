/*  
Formula SAE - BMS SLAVE v.1.0

Codice di Travagli Fabio, Lazzarin Nicola
Libreria di Travagli Fabio
ultima modifica: 01/04/2023

NOTE:
-caricabatterie: come riconoscere i dati per impostare le tensioni di soglia

*/

//pin definition 
#define error 2 //interrupt connected 
#define pd 3
#define cnv 4
#define TXmaster 8
#define RXmaster 9
#define cs 10
#define mosi 11
#define miso 12
#define clk 13

#define reg_v0 0x00 // indirizzo registro per leggere tensione cella 0
#define reg_v1 0x01 // indirizzo registro per leggere tensione cella 1
#define reg_v2 0x02 // indirizzo registro per leggere tensione cella 2
#define reg_v3 0x03 // indirizzo registro per leggere tensione cella 3
#define reg_v4 0x04 // indirizzo registro per leggere tensione cella 4
#define reg_v5 0x05 // indirizzo registro per leggere tensione cella 5

#define reg_t0 0x06 // indirizzo registro per leggere temperatura cella 0
#define reg_t1 0x07 // indirizzo registro per leggere temperatura cella 1
#define reg_t2 0x08 // indirizzo registro per leggere temperatura cella 2
#define reg_t3 0x09 // indirizzo registro per leggere temperatura cella 3
#define reg_t4 0x0A // indirizzo registro per leggere temperatura cella 4
#define reg_t5 0x0B // indirizzo registro per leggere temperatura cella 5


//libraries
#include <SoftwareSerial.h>
#include "AD7280.h"

//constant definition
SoftwareSerial rs485 = SoftwareSerial(RXmaster, TXmaster);  //serial channel for slaves comunication

//variables 
unsigned int id_slave = 0b0000000000000001; //001 bit che identificano questo slave
unsigned int id_master = 0b0000000000000000; //00 bit che identificano il master
unsigned int dato_ricevuto = 0b0000000000000000; // scrivere dato 16 bit appena ricevuto sul bus
short int state = 0; //move into different states
unsigned int cmd = 0b0000000000000000;  //16 bit for comunicate commands to slaves
int cont = 0; //charge-discharge cicle counter
int chipNumber = 4; //numero di chip AD7280
bool initAD7280 = false; //booleano per capire se l'inizializzazione è andata a buon termine, falso indica non andato a buon termine
bool selftestAD7280 = false; //booleano per capire se il selftest è andato a buon termine, falso indica non andato a buon termine
unsigned int tensioni0[4]; //array tensioni celle 0 per ogni chip
unsigned int tensioni1[4]; //array tensioni celle 1 per ogni chip
unsigned int tensioni2[4]; //array tensioni celle 2 per ogni chip
unsigned int tensioni3[4]; //array tensioni celle 3 per ogni chip
unsigned int tensioni4[4]; //array tensioni celle 4 per ogni chip
unsigned int tensioni5[4]; //array tensioni celle 5 per ogni chip
unsigned int temperature0[4]; //array temperature sensori 0 per ogni chip
unsigned int temperature1[4]; //array temperature sensori 1 per ogni chip
unsigned int temperature2[4]; //array temperature sensori 2 per ogni chip
unsigned int temperature3[4]; //array temperature sensori 3 per ogni chip
unsigned int temperature4[4]; //array temperature sensori 4 per ogni chip
unsigned int temperature5[4]; //array temperature sensori 5 per ogni chip
unsigned int vl; //variabile parametro threshold vlow
unsigned int vh; //variabile parametro threshold vhigh
unsigned int tl; //variabile parametro threshold tlow
unsigned int th; //variabile parametro threshold thigh
unsigned int dev; //variabile parametro device balance
bool c0 = false; // variabile parametro cel0 balance
bool c1 = false; // variabile parametro cel1 balance
bool c2 = false; // variabile parametro cel2 balance
bool c3 = false; // variabile parametro cel3 balance
bool c4 = false; // variabile parametro cel4 balance
bool c5 = false; // variabile parametro cel5 balance

//inizializzazione chip ad7280
AD7280 mystack(cs,cnv,chipNumber);

//-----------------------------------------SETUP
void setup() {
  //pin inizialization
  pinMode(TXmaster, OUTPUT);
  pinMode(RXmaster, INPUT);
  pinMode(error, INPUT);
  //pinMode(pd, OUTPUT); esiste già nella libreria
  //pinMode(cnv, OUTPUT); esiste già nella libreria

  //interrupt
  attachInterrupt(digitalPinToInterrupt(error), conversion, RISING); 

  //comunications
  Serial.begin(9600);
  rs485.begin(9600);
  initAD7280 = mystack.begin();  
  delay(100);
  selftestAD7280 = mystack.selfTest();
  delay(100);

}

//-----------------------------------------LOOP
void loop() {

  switch(state){
    //STATE 0 - inizialization
    case 0:    
      if ((initAD7280 && selftestAD7280)  == true){ //se l'inizializzazione è andata bene faccio i test
        Serial.println("inizializzazione chip completata con successo");
        Serial.println("selftest completato con successo");
        mystack.openwire();
        Serial.println("openwire test avviato");
        state = 1;
      }
      else{
        Serial.println("inizializzazione chip fallita, ritento");
        initAD7280 = mystack.begin();
        delay(100);
        selftestAD7280 = mystack.selfTest();
        delay(100);
      }

    break;

    //STATE 1 - ordinary operations 
    case 1:
      mystack.openwire();
      mystack.selfTest();
      mystack.readall(tensioni0[4],reg_v0);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni1[4],reg_v1);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni2[4],reg_v2);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni3[4],reg_v3);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni4[4],reg_v4);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni5[4],reg_v5);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature0[4],reg_t0);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature1[4],reg_t1);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature2[4],reg_t2);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature3[4],reg_t3);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature4[4],reg_t4);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature5[4],reg_t5);    //leggo i dati e li salvo nella variabile

      if(rs485.available()>0){ //se c'è un dato sul bus leggilo e salvalo sulla variabile
        dato_ricevuto = rs485.read();
        if((dato_ricevuto >> 13) == id_slave){ //shift verso destra di 13 bit e lo confronto con il suo id
          cmd = ((dato_ricevuto << 3) >> 6);//prima faccio lo shift verso sinistra di 3 e poi verso destra di sei per ottenere il dato con l'id 000
        }
        else{
          cmd = 0b0000000000000000;
        }
        if(cmd == 0b0000000000000001){//se il comando ricevuto è quello descritto invio tutti i valori di tensioni e temperatura
          for(int i = 0; i<4;i++){
            rs485.println(tensioni0[i] + id_master); //stampo il dato sul bus con l'id del master 000
            rs485.println(tensioni1[i] + id_master);
            rs485.println(tensioni2[i] + id_master);
            rs485.println(tensioni3[i] + id_master);
          }
          for(int i = 0; i<4;i++){
            rs485.println(temperature0[i] + id_master); //stampo il dato sul bus con l'id del master 000
            rs485.println(temperature1[i] + id_master);
            rs485.println(temperature2[i] + id_master);
            rs485.println(temperature3[i] + id_master);
          }
        }
        if(cmd == 0b0000000000000100){//se il comando ricevuto è quello descritto imposto stato = 0
          state = 0;
        }
        if(cmd == 0b0000000000001000){//se il comando ricevuto è quello descritto imposto stato = 1
          state = 1;
        }
        if(cmd == 0b0000000000001100){//se il comando ricevuto è quello descritto imposto stato = 2
          state = 2;
        }
    }

    break;

    //STATE 2 - caricabatterie
    case 2:
      mystack.readall(tensioni0[4],reg_v0);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni1[4],reg_v1);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni2[4],reg_v2);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni3[4],reg_v3);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni4[4],reg_v4);    //leggo i dati e li salvo nella variabile
      mystack.readall(tensioni5[4],reg_v5);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature0[4],reg_t0);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature1[4],reg_t1);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature2[4],reg_t2);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature3[4],reg_t3);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature4[4],reg_t4);    //leggo i dati e li salvo nella variabile
      mystack.readall(temperature5[4],reg_t5);    //leggo i dati e li salvo nella variabile

      if(rs485.available()>0){ //se c'è un dato sul bus leggilo e salvalo sulla variabile
        dato_ricevuto = rs485.read();
        if((dato_ricevuto >> 13) == id_slave){ //shift verso destra di 13 bit e lo confronto con il suo id
          cmd = ((dato_ricevuto << 3) >> 6);//prima faccio lo shift verso sinistra di 3 e poi verso destra di sei per ottenere il dato con l'id 000
        }
        else{
          cmd = 0b0000000000000000;
        }
        if(cmd == 0b0000000000000001){//se il comando ricevuto è quello descritto invio tutti i valori di tensioni e temperatura
          for(int i = 0; i<4;i++){
            rs485.println(tensioni0[i] + id_master); //stampo il dato sul bus con l'id del master 000
            rs485.println(tensioni1[i] + id_master);
            rs485.println(tensioni2[i] + id_master);
            rs485.println(tensioni3[i] + id_master);
          }
          for(int i = 0; i<4;i++){
            rs485.println(temperature0[i] + id_master); //stampo il dato sul bus con l'id del master 000
            rs485.println(temperature1[i] + id_master);
            rs485.println(temperature2[i] + id_master);
            rs485.println(temperature3[i] + id_master);
          }
        }
        if(cmd == 0b0000000000000100){//se il comando ricevuto è quello descritto imposto stato = 0
          state = 0;
        }
        if(cmd == 0b0000000000001000){//se il comando ricevuto è quello descritto imposto stato = 1
          state = 1;
        }
        if(cmd == 0b0000000000001100){//se il comando ricevuto è quello descritto imposto stato = 2
          state = 2;
        }
        if(cmd == 0b0000000000000010){//se il comando ricevuto è quello descritto aspetto i valori, poi avvio threshold e rispondo con il valore del booleano
          //if(rs485.available()>0){ //se c'è un dato sul bus leggilo e salvalo sulla variabile
          //  dato_ricevuto = rs485.read();
          //  if((dato_ricevuto >> 13) == id_slave){ //shift verso destra di 13 bit e lo confronto con il suo id
          //    cmd = ((dato_ricevuto << 3) >> 6);//prima faccio lo shift verso sinistra di 3 e poi verso destra di sei per ottenere il dato con l'id 000
          //  }
          //}
          //dichiarare booleano per verificare se è andato e rispedirlo alla seriale
          mystack.threshold(vl,vh,tl,th);
        }
        if(cmd == 0b0000000000000011){//se il comando ricevuto è quello descritto aspetto i valori e poi avvio balance
        //bla bla bla
        mystack.balance(dev,c0,c1,c2,c3,c4,c5);
        }
      }   
    break;

  }
}

//-------------------------------------PROCEDURES / FUN 

//interrupt procedure
void conversion(){
  Serial.println("Fase di conversione");
  mystack.readall(tensioni0[4],reg_v0);    //leggo i dati e li salvo nella variabile
  mystack.readall(tensioni1[4],reg_v1);    //leggo i dati e li salvo nella variabile
  mystack.readall(tensioni2[4],reg_v2);    //leggo i dati e li salvo nella variabile
  mystack.readall(tensioni3[4],reg_v3);    //leggo i dati e li salvo nella variabile
  mystack.readall(tensioni4[4],reg_v4);    //leggo i dati e li salvo nella variabile
  mystack.readall(tensioni5[4],reg_v5);    //leggo i dati e li salvo nella variabile
  Serial.println("Fase di conversione:letti i valori di tensioni");
  mystack.readall(temperature0[4],reg_t0);    //leggo i dati e li salvo nella variabile
  mystack.readall(temperature1[4],reg_t1);    //leggo i dati e li salvo nella variabile
  mystack.readall(temperature2[4],reg_t2);    //leggo i dati e li salvo nella variabile
  mystack.readall(temperature3[4],reg_t3);    //leggo i dati e li salvo nella variabile
  mystack.readall(temperature4[4],reg_t4);    //leggo i dati e li salvo nella variabile
  mystack.readall(temperature5[4],reg_t5);    //leggo i dati e li salvo nella variabile
  Serial.println("Fase di conversione:letti i valori di temperature");

}