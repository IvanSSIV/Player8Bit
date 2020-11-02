/****************************************************************************/
/* File: project_def.h                                                      */
/* Data creazione: 17/03/2020 												*/
/* Creato da: Adriano Olivetti - Ivan De Stefani							*/
/* Funzione: File header con definizioni globali a livello di progetto		*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

// nome apparato
#define NOME_APPARATO       "\n* Player-                      *"
// versione firmware
#define VERSIONE_FIRMWARE   "\n*    V 0.00.00 (gg-mm-aaaa)    *"

#define RIGA_SEPARAZIONE    "\n*------------------------------*"


//#define APPLICATION_MODE_CLASSIC

// tempi attivazione LED espressi in ms
//#define LED_ON_TIME  200L
//#define LED_OFF_TIME 800L

//DEFINIZIONE TIPO MACCHINA
//#define ARGO_EC


// definizione possibili valori di ritorno dalle funzioni di stato
#define FUNC_RET_EXEC  1 // in esecuzione
#define FUNC_RET_DONE  2 // finito 
#define FUNC_RET_ERROR 3 // avvenuto errrore
#define FUNC_RET_FAIL  4 // guasto irreversibile

// stabilisce ambiente AVR
#define ENV_AVR

#ifndef ENV_AVR
#define PROGMEM /**/
#endif