# Firmware ECU

## Indice

- [Contenuto del codice](#contenuto-del-codice)
- [Librerie](#librerie)
- [Codici di Test](#codici-di-test)
- [Autori e Copyright](#autori-e-copyright)

## Contenuto del codice
La presente repository contiene lo sviluppo del *main code* e delle librerie per il funzionamento dell'ECU dell'autovettura. La repository contiene, oltre al codice principale, le librerie per:

- Comunicazione CAN Bus con Inverter e BMS (Battery Management System)
- Gestione e acquisizione dati dei sensori
- Gestione dei led di stato dell'auto
- Comunicazione con la dashboard del volante
- Gestione e acquisizione dati per la Telemetria e GPS

# Librerie

## FSC_CAN_comunication

- FSC_BMSLib
> La libreria permette di comunicare con il BMS tramite il protocollo CANBus e di gestire la carica/scarica del pacco batterie e il circuito di *shutdown* per la sicurezza della vettura.

- FSC_InverterLib
> La libreria permette di comunicare con i quattro Inverter tramite il protocollo CANBus e di gestire le impostazioni degli stessi.

## FSC_Dashboard
> Libreria per gestire le informazioni acquisite dai sensori, dal BMS e Inverter, dalla Telemetria e inviarle al micro della dashboard per essere visionati a schermo.

## FSC_LedCheck
> Libreria che gestisce il reset e setup dei led di stato presenti sulla motherboard.

## FSC_SensorLib
> Libreria che prende i dati dai sensori e li manipola tramite conversioni. La libreria procede anche ad un check sull'attendibilità dei dati e provvede a salvarli in una struttura dati.

## Codici di Test
Nella cartella vengono inseriti tutti i codici di test effettuati durante la fase di prova dei componenti.

# Autori e Copyright
Per ulteriori modifiche, commentare ed aggiornare il presente documento per tenere traccia delle modifiche.
Il presente materiale è coperto in Italia dalla legge sul diritto d'autore (l. 633/1941).
## Autori
- Leonardo Cirelli


