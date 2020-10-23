/****************************************************************************/
/* File: IO_mapping.h                                                       */
/* Data creazione: 24/03/2020                                               */
/* Creato da: Adriano Olivetti - Ivan De Stefani							*/
/* Funzione: Mappatura I/O a livello PLC; effettua delle define mnemoniche	*/
/*           degli ingressi/uscite del PLC specifiche dell'applicazione     */
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

#define INGRESSO_MANO  PLC_work.X[1]
#define INGRESSO_TAPPO PLC_work.X[3]

#define LED_SCIVOLO    PLC_work.Y[23]
#define LED_TRAMOGGIA  PLC_work.Y[2]
