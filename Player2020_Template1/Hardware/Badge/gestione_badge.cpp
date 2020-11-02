/****************************************************************************/
/* File:           gestione_badge.cpp                                       */
/* Data creazione: 06/05/2020           									*/
/* Creato da:      Ivan De Stefani           								*/
/* Funzione:       Modulo di gestione della lettura del badge e delle       */
/*				   autorizzazioni tramite lo stesso.						*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

#include "Arduino.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "My_Include/mydef.h"
#include "gestione_badge.h"

/*******************************************************************/
/* BOOL badge_char_available(void)                                 */
/*_________________________________________________________________*/
/* Funzione che controlla se è stato passato un badge.             */
/* Restituisce TRUE se ci sono caratteri da leggere sulla seriale, */
/* FALSE se non è stato passato nessun badge.                      */
/* Parametri: nessuno                                              */
/*******************************************************************/
BOOL badge_char_available(void)
{
	BOOL retval = FALSE;

	if (Serial2.available())
		retval = TRUE;

	return retval;
}


/*******************************************************************/
/* void badge_init_LL(uint32_t baud)                               */
/*_________________________________________________________________*/
/* Funzione che inizializza la seriale Serial2 a basso livello.    */
/* Parametri: baud rate a cui inizializzare la seriale.            */
/*******************************************************************/
void badge_init_LL(uint32_t baud)
{
	Serial2.begin(baud);
	Serial2.flush();
}

/*-------------------------------------------------------------------*/
/* function badge_read_char()                                        */
/* Legge un carattere dalla seriale del badge . In uscita            */
/* restituisce il carattere letto oppure 0xFFFF se non ci sono dati  */
/*-------------------------------------------------------------------*/
uint16_t badge_read_char(void)
{
	if (Serial2.available())
	{
		return Serial2.read();
	}
	else
	{
		return 0xFFFF;
	}
}

/***********************************************************************/
/* void get_badge_info(char * msg, BOOL auth_vec[])                    */
/*_____________________________________________________________________*/
/* Funzione che inserisce in un vettore di booleani le autorizzazioni. */
/* al prelievo dai vari settori del badge letto.                       */
/* Parametri: msg: stringa di risposta dal server                      */
/*            auth_vec[]: vettore di booleani per informazioni         */
/***********************************************************************/
BOOL get_badge_info(char* msg, BOOL auth_vec[])
{
	/* PSEUDO-CODE
		BOOL retval;
		retval = FALSE;
		int pos = 0;
		for(pos = 0; pos < MAX_SEC_NUM; pos++)
		{
		  if(vett_auth[pos] == '1')
		   {
			  auth_vec[pos] == TRUE;
		   }
		}
		if(vett_auth[MAX_SEC_NUM] == '*')
		{
			retval = TRUE;
			// Il badge è un autorizzatore
		}

		return retval;
	*/
}