#include "..\Lanzi_8bit_2560.h"
#include "debug_HW.h"

#define GPIO_HW_TP0 14
#define GPIO_HW_TP1 16
#define GPIO_HW_TP2 15
#define GPIO_HW_TP3 17


/*****************************************************************/
/* void dbg_HW_init(void)                                        */
/*---------------------------------------------------------------*/
/* Inizializzazione dei test point di debug hardware.            */
/* Collegandosi direttamente ai piedini del microcontrollore,    */
/* otteniamo una velocità di risposta di un ordine di grandezza  */
/* minore rispetto alla velocità di comunicazione dei transistor */
/* presenti sulle uscite della scheda (circa 15us).              */
/*****************************************************************/
void dbg_HW_init(void)
  {
     uint8_t temp;
     cli();
     // leggi valore DDRH
     temp = DDRH;
     // pone PH2,PH3,PH4,PH5 in output
     temp |= 0x3C;
     DDRH = temp;
     // pone PH2,PH3,PH4,PH5 a zero
     PORTH = PORTH & 0xC3;
     sei();
  }


/* Attiva il test point HW TP0 */
/* impiega circa 480ns         */
void dbg_HW_TP0_ON(void)
{
    cli();
    // metti PH2 a 1
    PORTH = PORTH | 0x04;
    sei();
}

/* Disattiva il test point HW TP0 */
/* impiega circa 480ns            */
void dbg_HW_TP0_OFF(void)
{
    //uint8_t temp;

    cli();
    // metti PH2 a 0
    PORTH = PORTH & 0xFB;
    sei();
}

void dbg_HW_TP1_ON(void)
{
    cli();
    // metti PH3 a 1
    PORTH = PORTH | 0x08;
    sei();
}
void dbg_HW_TP1_OFF(void)
{
    cli();
    // metti PH3 a 0
    PORTH = PORTH & 0xF7;
    sei();
}


/* Attiva il test point HW TP2 */
/* impiega circa 480ns         */

void dbg_HW_TP2_ON(void)
{
    cli();
    // metti PH4 a 1
    PORTH = PORTH | 0x10;
    sei();
}

/* Disattiva il test point HW TP2 */
/* impiega circa 480ns            */

void dbg_HW_TP2_OFF(void)
{
    //uint8_t temp;

    cli();
    // metti PH4 a 0
    PORTH = PORTH & 0xEF;
    sei();
}


/* Attiva il test point HW TP3 */
/* impiega circa 480ns         */

void dbg_HW_TP3_ON(void)
{
    cli();
    // metti PH5 a 1
    PORTH = PORTH | 0x20;
    sei();
}

/* Disattiva il test point HW TP3 */
/* impiega circa 480ns            */

void dbg_HW_TP3_OFF(void)
{
    //uint8_t temp;

    cli();
    // metti PH5 a 0
    PORTH = PORTH & 0xDF;
    sei();
}
