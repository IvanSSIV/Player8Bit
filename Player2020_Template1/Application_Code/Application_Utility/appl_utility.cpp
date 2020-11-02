/****************************************************************************/
/* File: appl_utility.cpp                                                   */
/* Data creazione: 17/03/2020  												*/
/* Creato da: Adriano Olivetti - Ivan De Stefani                        	*/
/* Funzione: Modulo che contiene funzioni di utilità all'applicazione		*/
/*			 principale.    												*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/
#define MODULE_APPL_UTILITY
#include "player_header.h"
// definizioni globali


// definizione variabili
void appl_init_title(void)
{
    debug_print((char*)"\n\r");
    debug_print((char*)RIGA_SEPARAZIONE);
    debug_print((char*)NOME_APPARATO);
    debug_print((char*)VERSIONE_FIRMWARE);
    debug_print((char*)RIGA_SEPARAZIONE);
}

#undef MODULE_APPL_UTILITY