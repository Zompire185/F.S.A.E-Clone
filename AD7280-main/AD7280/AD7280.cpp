

//libreria per la comunicazione con l' integrato AD7280, scritta da Fabio Travagli 25/04/2023


#include "AD7280.h"
#include "Arduino.h"
#include "SPI.h"
#define BLANK 0xF800030A                                    


/////////////////////////////////////////////////////public//////////////////////////////////////////////////////////////

    AD7280::AD7280(int sspin,int cnvstpin, int chipNumber){     //costruttore
      deviceNumber=chipNumber;    //memorizzo il numero di dispositivi nello stack
      convpin=cnvstpin;           //memorizzo il pin del segnale di conversione 
      cspin=sspin;                //memorizzo il pin di chipselect
      digitalWrite(cspin,HIGH);   //impostazione a livello logico alto
      pinMode(cspin,OUTPUT);      //definizione a stato di uscita, quest'ordine permette immediatamente di impostare il pin di CS con un pullup prima di avere un uscita pilotata direttamente 
      pinMode(cnvstpin,OUTPUT);     //definizione a stati di uscita
      digitalWrite(cnvstpin,HIGH);  //inizializzazione del pin di conversione
    }
    
  bool AD7280::begin(){         //inizializzazione
      SPI.begin();                //inizializzazione della comunicazione SPI
      unsigned long int value = 0x01C3B7E2;     //il comando viene dalla scheda tecnica dell' integrato e permette di impostare gli indirizzi degli i integrati e impostare la resistenza di terminazione dei termistori
      transfer (value);           //il comando viene trasmesso 
      value=0x038716CA;               //comando preso dalla scheda tecnica dell' integrato che richiede un operazione di lettura del registro di controllo
      transfer(value);            //il comando viene trasmesso
      for(int i=0;i<deviceNumber;i++){//per ognuno dei chip dello stack
        value = blank();    //ogni volta che viene caricato un messaggio vuoto viene ricevuto il dato del registro di controllo richiesto
        byte address=0x00;              //variabile che contiene l'indirizzo
        address=(value>>26)&0x3F;       //viene estratto l'indirizzo del dispositivo
        
        if(address!= addres(i)){      //controlla se l'indirizzo resistuito corrisponde a quello generato
          return false;               //se non corrisponde, ritorna falso, segnalando che l'inizializzazione è fallita
        }
      }
      for(int i=0;i<deviceNumber;i++){   //inizializzazione del registro di allert
        if(i==deviceNumber-1){
          write(i,0x13,0x40);            //il dispositivo più lontano viene configurato come uscita statica
        }else{
          write(i,0x13,0xC0);           //i dispositivi intermedi vengono configurati per passare il segnale  
        }
      }
      go=true;                        //abilitazione all' esecuzione di altri comandi da parte degli integrati
      return true;                    //se tutti gli indirizzi sono andati a buon fine allora conferma l'inizializzazione 
    }
    
  
   void AD7280::readall(unsigned int result[],int input){   //operazione di lettura di un registro da tutti i dispositivi
      if(!go){                                      //qualora non ci sia stata una corretta inizializzazione, viene impedita l'esecuzione di questo comando
        for(int i=0;i<6;i++){                       //il buffer dei risultati viene riempito con il massimo possibile
          result[i]=0xFFFF;
        }
        return;
      }

      if(input<0&input>12){
        return;
      }
      writeall(0x1C,(byte)input);                   //viene inserito il registro desiderato nel registro di lettura in ogni dispositivo
      digitalWrite(convpin, LOW);                   //viene inizializzata la conversione
      delay(1);                                     //viene atteso un tempo per la conversione 
      digitalWrite(convpin, HIGH);                  //viene ripristinato lo stato iniziale del pin di conversione
      for(int i=0;i<deviceNumber;i++){              //per ogni dispositivo nella catena
        unsigned long int value = blank();          //un frame vuoto viene mandato per poter ricevere i dati
        if(value==crc(value,22)){                   //se il crc è corretto
          result[i]=(unsigned int)(value&0x007FF800)>>11; //viene caricato il risultato sull' array
        }else{
          go=false;                                 //altrimenti viene tolta l'abilitazione a procedere 
          result[i]=0xFFFF;                         //se viene rilevato un errore viene scritto un valore fuori dal range operativo
          }
      }
    
   }
  
   void AD7280::openwire(){           //operazione di verifica del' integrità dei fili di sense consiste nell' attivare le uscite del bilanciamento e se c'è un filo interrotto, l'ingresso eccede il limite d'ingresso 
      if(pattern){            //in maniera alternata per equilibrare la dissipazione termica delle resistenze di bilanciamento
        writeall(0x14,0xA8);  //viene modificato il registro delle uscite  con il pattern 1/0/1/0/1/0
      }else{
        writeall(0x14,0x54);   //viene modificato il registro delle uscite  con il pattern 0/1/0/1/0/1
      }
      pattern = !pattern;    //il pattern successivo sarà invertito
      delay(1);              //si attende un millisecondo
      writeall(0x14,0x00);   //si spengono le uscite 
   }
  
   void AD7280::balance(int device, bool cel0, bool cel1, bool cel2, bool cel3, bool cel4, bool cel5){     //modifica il registro delle uscite di bilanciamento, attivando o disattivando le uscite
      if(!go){                                                                                      //qualora non ci sia stata una corretta inizializzazione, viene impedita l'esecuzione di questo comando
        return;
      }
      byte value=((cel5*0x80)|(cel4*0x40)|(cel3*0x20)|(cel2*0x10)|(cel1*0x08)|(cel0*0x04))&0xFC;   //viene determinato il valore del registro in funzione dello stato delle uscite
      write(device,0x14,value);                                                                    //viene modificato il registro
   }
  
   bool AD7280::threshold(unsigned int Vlow, unsigned int Vhigh, unsigned int Tlow, unsigned int Thigh){    //funzione che imposta le soglie di allerta
      if(!go){                                               //qualora non ci sia stata una corretta inizializzazione, viene impedita l'esecuzione di questo comando e viene restituito falso, a indicare l'errore
        return false;
      }
      if((Vlow<256)&(Vhigh<256)&(Tlow<256)&(Thigh<256)){    //se è più piccolo di un byte
        writeall(0x0F,byte(Vhigh));                         //scrive ai registri corrispondenti 
        writeall(0x10,byte(Vlow));
        writeall(0x11,byte(Thigh));
        writeall(0x12,byte(Tlow));
        return true;                                        //se l'operazione ha successo, viene restituito vero
      }else{
        return false;                                       //altrimenti se l'input non è rispettato viene restituito falso
      }
   }
  
  bool AD7280::selfTest(){            //funzione di selftest della conversione 
    bool error =false;                 //variabile booleana che memorizza lo stato di errore
    unsigned int result;              //variabile che memorizza il risultato della conversione
    writeall(0x0D,0xC0);               //impostazione dell'input al selftest
    writeall(0x1C,0x0C);                   //viene inserito il registro del risultato del selftest nel registro di lettura in ogni dispositivo
    digitalWrite(convpin, LOW);                   //viene inizializzata la conversione
    delay(1);                                     //viene atteso un tempo per la conversione 
    digitalWrite(convpin, HIGH);                  //viene ripristinato lo stato precedente del pin
    for(int i=0;i<deviceNumber;i++){              //per ogni dispositivo dello stack
      unsigned long int value = blank();          //un frame vuoto viene mandato per poter ricevere i dati
      if(value==crc(value,22)){                   //se il crc è corretto
        result=(unsigned int)(value&0x007FF800)>>11; //viene ricavato il risultato
        if(result<970|result>990){                //se è al di fuori del range operativo 
          error=true;                          //viene settato il bit di errore
        }
      }else{
        error=true;                         //se viene rilevato un errore viene scritto un valore fuori dal range operativo
        }
    }
    writeall(0x0D,0x00);                     //l'input viene reimpostato all' ingresso delle celle e delle temperature
    go!=error;                               //la variabile go assume il valore dell' errore, 
     return error;                           //viene restituito lo stato del sistema
  }


  bool AD7280::Run(){
    return go;
  }
    
  ///////////////////////////////////////////////private//////////////////////////////////////////////////////////
  
  unsigned long int AD7280::transfer (unsigned long int value){   //funzione che scrive un messaggio e restituisce il messaggio mandato dai chip
    byte arr[4]={0};                //array di byte con il dato in ingresso
    unsigned long int res[4]={0};   //array che contiene il valore restituito dalla funzione transfer
    arr[0] = value & 0xFF;          //viene riempito
    arr[1] = (value >> 8) & 0xFF;
    arr[2] = (value >> 16) & 0xFF; 
    arr[3] = (value >> 24) & 0xFF;
  
    digitalWrite(cspin, LOW);       //il pin CS viene attivato

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1)); //inizio della transazione dichiarando Fclock=1Mhz e la modalità della trasmissione
  
    res[3]=SPI.transfer(arr[3]);    //viene trasmesso il comando e riempito l'array dei risultati
    res[2]=SPI.transfer(arr[2]);
    res[1]=SPI.transfer(arr[1]);
    res[0]=SPI.transfer(arr[0]);

    SPI.endTransaction();
  
    digitalWrite(cspin, HIGH);       //il pin CS viene disattivato
  
    return (res[0]|(res[1]<<8)|(res[2]<<16)|(res[3]<<24))&0xFFFFFFFF;
  }
  
  void AD7280::write(int address,byte reg, byte data){  //operazione di scrittura di un dato in un registro
      transfer(command(address,reg,data,false));
  }
  
   void AD7280::writeall(byte reg, byte data){     //operazione di scrittura su tutti i dispositivi di un dato in un registro
    transfer(command(0,reg,data,true));
  }
    
    byte AD7280::addres(int x) {         //dato un indice ricava l'indirizzo corrispondente tenendo conto dell' ordine invertito
      return ((x & 0x01) * 0x10) | ((x / 2 & 0x01) * 0x08) | ((x / 4 & 0x01) * 0x04) | ((x/ 8 & 0x01) * 0x02) | ((x / 16 & 0x01) * 0x01);
  }
  
  
   unsigned long int AD7280::blank(){     //trasmette un frame vuoto
       unsigned long int value = transfer(BLANK);
       if (value == crc(value, 22)&value!=0x00000000) {
           return value;
       }else {
           go = false;
           return 0xFFFFFFFF;
       }
    }
  
  unsigned long int AD7280::crc(unsigned long int value, int bitnum) {  //funzione che calcola il crc di un comando dato
    bool xor_[5] = {0};       //stato di uscita degli xor
    bool CRC[8] = {0};        //risultato del crc
    bool data_in[32] = {0};   //array che contiene il dato in ingresso

    
  
    for (int i = 0; i < 32; i++){  
        data_in[i] = (bool)((int)(value/pow(2,i))%2);    //viene riempito l'array 
    }
    for (int i = 0; i < bitnum; i++) {    //con un ciclo viene eseguito il calcolo del crc
        xor_[4] = CRC[4] ^ CRC[7];
        xor_[3] = CRC[2] ^ CRC[7];
        xor_[2] = CRC[1] ^ CRC[7];
        xor_[1] = CRC[0] ^ CRC[7];
        xor_[0] = data_in[31-i] ^ CRC[7];
        CRC[7]= CRC[6];
        CRC[6]= CRC[5];
        CRC[5]= xor_[4];
        CRC[4]= CRC[3];
        CRC[3]= xor_[3];
        CRC[2]= xor_[2];
        CRC[1]= xor_[1];
        CRC[0] = xor_[0];
    }
        byte crc = ((CRC[7] * 0x80) | (CRC[6] * 0x40) | (CRC[5] * 0x20) | (CRC[4] * 0x10) | (CRC[3] * 0x08) | (CRC[2] * 0x04) | (CRC[1] * 0x02) | (CRC[0] * 0x01));   //viene generato un byte contenente il crc
        if(bitnum==21){
            return (value | ((crc << 3)&0x000007F8));    //viene aggiunto il crc al messaggio iniziale 
        }else {
            return (value | ((crc << 2) & 0x000003FC));    //viene aggiunto il crc al messaggio iniziale 
        }
  }     
  
   unsigned long int AD7280::command(int address,byte reg, byte data, bool all) {  //genera il comando dato il device address, il registro, il dato e se il messaggio è rivolto a tutti i dispositivi
    unsigned long int result = 0x00000000;              //comando risultante 
    result = result | ((unsigned long int)addres(address) << 27) | ((unsigned long int)reg << 21) | (data << 13) | (all << 12); //composizione del comando con indirizzo, registro, dati
    result = crc(result,21);    //viene aggiunto la parte di crc 
    return result | 0x00000002;     //viene restituito il valore aggiungendo la porzione finale del comando (010)
  }
  
