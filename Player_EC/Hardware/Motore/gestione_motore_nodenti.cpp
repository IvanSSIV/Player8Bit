#include "..\Lanzi_8bit_2560.h"
#include "gestione_motore_nodenti.h"


/************************************************/
/* void LL_motor_init(void)                     */
/*----------------------------------------------*/
/* Inizializzazione del motore a basso livello. */
/************************************************/
void LL_motor_init(void)
  {
    cli();
        uint8_t temp;
        temp = DDRB;
        temp |= 0x80;   // 1000 0000; metto PB7 ad OUTPUT
        DDRB = temp;
        temp = DDRC;
        temp |= 0x02;   // 0000 0010; metto PC1 ad OUTPUT
        DDRC = temp;
    sei();
  }

/**************************************************/
/* void motor_START(void)                         */
/*------------------------------------------------*/
/* Commutazione dell'uscita del motore al livello */
/* logico ALTO.                                   */
/**************************************************/
void motor_START(void)
{
    PORTB = PORTB | 0x80;
}

 
/**************************************************/
/* void motor_STOP(void)                          */
/*------------------------------------------------*/
/* Commutazione dell'uscita del motore al livello */
/* logico BASSO.                                  */
/**************************************************/
void motor_STOP(void)
{
    PORTB = PORTB & 0x7F;
}

/***************************************************/
/* void motor_FWD(void)                            */
/*-------------------------------------------------*/
/* Commutazione dell'uscita del relè di inversione */
/* motore a livello logico BASSO.                  */
/***************************************************/
void motor_FWD(void)
{
  cli();
  PORTC = PORTC & 0xFD;
  sei();
}

/***************************************************/
/* void motor_REV(void)                            */
/*-------------------------------------------------*/
/* Commutazione dell'uscita del relè di inversione */
/* motore a livello logico ALTO.                   */
/***************************************************/
void motor_REV(void)
{
  cli();
  PORTC = PORTC | 0x02;
  sei();
}