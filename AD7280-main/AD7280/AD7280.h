#ifndef AD7280_h
#define AD7280_h
#include <Arduino.h>

class AD7280{
  public:
    AD7280(int sspin,int cnvstpin, int chipNumber);//costruttore

    bool begin();         //inizializzazione 
    void readall(unsigned int result[],int input);   //operazione di lettura di un registro da tutti i dispositivi
    void openwire();           //operazione di verifica del' integrità dei fili di sense consiste nell' attivare le uscite del bilanciamento e se c'è un filo interrotto, l'ingresso eccede il limite d'ingresso
    void balance(int device, bool cel0, bool cel1, bool cel2, bool cel3, bool cel4, bool cel5);     //modifica il registro delle uscite di bilanciamento, attivando o disattivando le uscite
    bool threshold(unsigned int Vlow, unsigned int Vhigh, unsigned int Tlow, unsigned int Thigh);    //funzione che imposta le soglie di allerta
    bool selfTest();            //funzione di selftest della conversione 
    bool Run();                 //funzione che indica l'essenza di errori
    
  private:

    bool go=false;      //variabile che memorizza se è avvenuta un inizializzazione con successo
    bool pattern=false; //durante un operazione di controllo per la rottura di un filo, ricorda il pattern precedente e li alterna 
    int cspin;          //memorizza i pin di chip select
    int deviceNumber;   //memorizza il numero di chip collegati
    int convpin;        //memorizza il bit di conversione 

    unsigned long int transfer (unsigned long int value);   //funzione che scrive un messaggio e restituisce il messaggio mandato dai chip
    void write(int address,byte reg, byte data);  //operazione di scrittura di un dato in un registro
    void writeall(byte reg, byte data);     //operazione di scrittura su tutti i dispositivi di un dato in un registro
    byte addres(int x);          //dato un indice ricava l'indirizzo corrispondente tenendo conto dell' ordine invertito
    unsigned long int  blank();     //trasmette un frame vuoto
    unsigned long int crc(unsigned long int value, int bitnum);   //funzione che calcola il crc di un comando dato
    unsigned long int command(int address,byte reg, byte data, bool all);  //genera il comando dato il device address, il registro, il dato e se il messaggio è rivolto a tutti i dispositivi
  
};



#endif
