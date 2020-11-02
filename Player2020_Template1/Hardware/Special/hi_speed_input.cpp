#include "Arduino.h"
#include "stdint.h"
#include "stdio.h"
#include "My_Include/mydef.h"
#include "My_Debug/Debug_HW/debug_HW.h"
#include "My_Debug/Debug_Utility/debug_utility.h"
static volatile uint16_t ISR_count;
static volatile uint16_t ISR_count_max;
static volatile BOOL ISR_diff;
static volatile uint8_t ISR_event;

/*************************************************/
/* void hi_speed_init_LL(void)                   */
/*-----------------------------------------------*/
/* Inizializzazione del timer 5 dell'AtMega2560  */
/* per la gestione della lettura dell'ingresso   */
/* assegnato al segnale di caduta del tappo con  */
/* timer interrupt.                              */
/*************************************************/
void hi_speed_init_LL(void)
  {
		// stop timer 5
		TCCR5A = 0;
		TCCR5B = 0;
		// reset timer 5 counter
		TCNT5 = 0;
		// 
		OCR5A = 16; // (31250 = 500 ms --> 16 = 250 us) compare match register 16MHz/256/2Hz
		TCCR5A |= (1 << WGM51); // CTC mode
		TCCR5B |= (1 << CS52); // 256 prescaler

		TIMSK5 |= (1 << OCIE5A); // enable timer compare interrupt
  }

/****************************************************/
/* BOOL hi_speed_get_event(void)                    */
/*--------------------------------------------------*/
/* Funzione che notifica l'applicazione principale  */
/* dell'avvenuta caduta di un tappo come evento     */
/* gestito dall'ISR.                                */
/****************************************************/
BOOL hi_speed_get_event(void)
  {
	 BOOL retval;
	 
	 retval = FALSE;

	 if (ISR_event)
	   {
		  cli();
		  ISR_event--;
		  sei();
		  retval = TRUE;
	   }
	 return retval;
  }

/********************************************************************************************/
/* ISR(TIMER_COMPA_vect)                                                                    */
/*------------------------------------------------------------------------------------------*/
/* Questa ISR campiona con un rate pari al valore del timer 5 (attualmente circa 250 us)    */
/* e accumula in una variabile il numero di campionamenti (detti anche tick) in cui viene   */
/* trovato il segnale a 1.                                                                  */
/* L'errore di campionamento strutturale è pari a 1 tick all'inizio del segnale e 1 tick    */
/* alla fine del segnale. Con un campionamento a 250 us, un segnale di 2ms ha un numero di  */
/* campionamenti teorico pari a 8. Il numero di campionamenti garantito, invece è 7.        */
/* Per migliorare la risoluzione, è necessario aumentare la risoluzione del timer per avere */
/* interrupt più fitti. Per una buona risoluzione, si utilizzano 10 campionamenti per la    */
/* lunghezza di un segnale. Il limite superiore di questa soluzione è che la frequenza      */
/* degli ISR del timer non sia troppo fitta da saturare completamente il processore.        */
/* N.B. Controllare sempre con l'oscilloscopio la durata della ISR.                         */
/********************************************************************************************/
ISR(TIMER5_COMPA_vect)
   {
	  TCNT5 = 0;
	  dbg_HW_TP0_ON();

	  if (PINF & 0x08)
	    {
		  if (ISR_diff)
		    {
			  ISR_count_max = ISR_count;
			  ISR_event++;
		    }
		  ISR_diff = FALSE;
		  ISR_count = 0;
	    }
	  else
	    {
		  ISR_diff = TRUE;
		  ISR_count++;
	    }
      dbg_HW_TP0_OFF();
   }
