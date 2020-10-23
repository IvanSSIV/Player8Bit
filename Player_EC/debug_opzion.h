/************************************************************************************/
/* File: debug_opzion.h                                                             */
/* Data creazione: 17/03/2020  						            */
/* Creato da: Adriano Olivetti - Ivan De Stefani			            */
/* Funzione: File header che contiene le definizioni per l'abilitazione	            */
/*			 modulare delle stampe di debug.		            */
/*                                                                                  */
/* Changelog:									    */
/*		08/04/2020 IDS  Aggiunte definizioni per attivazione/disattivazione */
/*						debug per ogni singolo modulo.	    */
/*                                                                                  */
/************************************************************************************/

// Costante per debug sempre acceso all'inizializzazione della macchina
#define DBG_ALWAYS_ON true

/***********************************************/
/*     ABILITAZIONI TEST POINT HARDWARE        */
/***********************************************/

// abilita TP hardware su main PLC (usa TP0)
//#define HW_TP_PLC_MAIN 

// abilita TP hardware su PLC_RW (usa TP2 e TP3)
//#define HW_TP_PLC_RW

// abilita TP hardware su main RTC (usa TP1)
//#define HW_TP_RTC_MAIN

// abilita TP hardware su main LCD (usa TP0)
//#define HW_TP_LCD_MAIN

#define DEBUG_IDENT_NONE	0
#define DEBUG_IDENT_L1		3
#define DEBUG_IDENT_L2		4
#define DEBUG_IDENT_L3		5

// livello identazione sotto stati (primo livello)
#define DEBUG_IDENT_SUB_STATE 3

