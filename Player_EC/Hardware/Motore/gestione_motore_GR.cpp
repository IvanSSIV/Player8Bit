#include "Arduino.h"
#include "project_def.h"
//#include <Application_Code/project_def.h>
#include <debug_opzion.h>
#include
#include <My_Debug/Debug_Serial/debug_serial.h>
#include <My_Debug/Debug_Utility/debug_utility.h>
#include "gestione_motore_nodenti.h"
#include <My_Debug/Serial_Command/serial_command.h>


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