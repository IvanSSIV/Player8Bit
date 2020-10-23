/****************************************************************************/
/* File: timing.cpp                                                         */
/* Data creazione: 31/03/2020  												*/
/* Creato da: Adriano Olivetti - Ivan De Stefani							*/
/* Funzione: Gestisce le basi tempi del funzionamento della macchina		*/
/*			 basandosi sul timer millis() di Arduino.       				*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

#define MODULE_TIMING
#include "..\Lanzi_8bit_2560.h"

// variabili locali
static uint32_t basetempi_10ms;  // conteggio ms per base tempi 10ms
static uint32_t basetempi_100ms; // conteggio ms per base tempi 100ms
static uint32_t basetempi_1s;    // conteggio ms per base tempi 1sec

void basetempi_RTOS_init(void)
  {
      basetempi_10ms =
      basetempi_100ms =
      basetempi_1s = RTOS_get_tick_1ms();
  }

/*--------------------------------------------------------*/
/* function RTOS_get_tick_1ms()                           */
/* Restituisce in numero di tick a 1ms dall'ultimo reser  */
/*--------------------------------------------------------*/
uint32_t RTOS_get_tick_1ms(void)
  {
    return millis();
  }

/*----------------------------------------------------------*/
/* function basetempi_RTOS_main()                           */
/* Gestisce la base tempi a 10,100,1000ms legata alla base  */
/* del RTOS o al timer per sistemi privi di RTOS            */
/* Va posta all'inizio del loop principale del programma    */
/*----------------------------------------------------------*/
void basetempi_RTOS_main(void)
{
    uint32_t ms;

    // leggi i tick a 1ms
    ms = RTOS_get_tick_1ms();

      // vedi se trascorsi 10ms
    if ((ms - basetempi_10ms) >= 10L)
    {
        basetempi_10ms = ms;
        flg_bt_10ms = TRUE;
    }

    // vedi se trascorsi 100ms
    if ((ms - basetempi_100ms) >= 100L)
    {
        basetempi_100ms = ms;
        flg_bt_100ms = TRUE;
    }

    // vedi se trascorsi 1sec
    if ((ms - basetempi_1s) >= 1000L)
    {
        basetempi_1s = ms;
        flg_bt_1s = TRUE;
    }
}

/*----------------------------------------------------------*/
/* function basetempi_RTOS_clear_flg()                      */
/* Azzera i flag delle basetempi a 10,100,1000ms            */
/* Va posta alla fine del loop principale del programma     */
/*----------------------------------------------------------*/
void basetempi_RTOS_clear_flg(void)
{
    flg_bt_10ms = FALSE;
    flg_bt_100ms = FALSE;
    flg_bt_1s = FALSE;
}


#undef MODULE_TIMING