/****************************************************************************/
/* File: project_def.h                                                      */
/* Data creazione: 17/03/2020 						    */
/* Creato da: Adriano Olivetti - Ivan De Stefani			    */
/* Funzione: File header con definizioni globali a livello di progetto	    */
/*									    */
/* Changelog:								    */
/*									    */
/*									    */
/****************************************************************************/
#define MACHINE_TYPE ARGO_EC


//#define APPLICATION_MODE_CLASSIC


#define PLC_MAX_X  24
#define PLC_MAX_Y  24

// tempi attivazione LED espressi in ms
#define LED_ON_TIME  200L
#define LED_OFF_TIME 800L


// Stabilisce ambiente Arduino
#define ENV_ARDUINO

// stabilisce ambiente AVR
#define ENV_AVR

#ifndef ENV_AVR
#define PROGMEM /**/
#endif

// Definizioni variabili in funzione del tipo di macchina per cui si vuole compilare
#if MACHINE_TYPE == ARGO_EC
#define MAX_SEC_NUM	1            // Numero massimo di settori possibili su una singola macchina
#define MAX_PIATTI      1            // numero di piatti della macchina
#define MAX_SETTORI     1            // numero massimo settori
#elif MACHINE_TYPE == ARGO_KD
#define MAX_SEC_NUM	20           // Numero massimo di settori possibili su una singola macchina
#define MAX_PIATTI      20           // numero di piatti della macchina
#define MAX_SETTORI     20           // numero massimo settori
#elif MACHINE_TYPE == ARGO_LK
#define MAX_SEC_NUM	39           // Numero massimo di settori possibili su una singola macchina
#define MAX_PIATTI      39           // numero di piatti della macchina
#define MAX_SETTORI     39           // numero massimo settori
#elif MACHINE_TYPE == ARGO_GR
#define MAX_SEC_NUM	1	     // Numero massimo di settori possibili su una singola macchina
#define MAX_PIATTI      1            // numero di piatti della macchina
#define MAX_SETTORI     1            // numero massimo settori
#elif MACHINE_TYPE == ARGO_XL
#define MAX_SEC_NUM	15	     // Numero massimo di settori possibili su una singola macchina
#define MAX_PIATTI      15           // numero di piatti della macchina
#define MAX_SETTORI     15           // numero massimo settori
#endif

#define MAX_SIZE_NUM_COMPUTER  8    // dimensione campo NrComputer
#define MAX_SIZE_NOME_COMPUTER 8    // dimensione campo NomeComputer

// Definizione per compilazione con debug hardware
#define DEBUG_HARDWARE		1

//Definizione per compilazione condizionale con messaggi di debug
#define DEBUG_VERSION		1

// definizione nomi file usati per memorizzazione dati su SD
#define NOME_FILE_CONFIG     (char *)"NWCONFIG.DAT"
#define NOME_FILE_BOOT       (char *)"BOOTCFG.DAT"
#define NOME_FILE_ANAGRAFICA (char *)"ANAG_NW.DAT"
#define NOME_FILE_APERTURA   (char *)"APERTURA.DAT"
#define NOME_FILE_CHIUSURA   (char *)"CHIUSURA.DAT"
#define NOME_FILE_RICARICA   (char *)"RICARICA.DAT"
#define NOME_FILE_ANAGCN     (char *)"ANAGCN.DAT"