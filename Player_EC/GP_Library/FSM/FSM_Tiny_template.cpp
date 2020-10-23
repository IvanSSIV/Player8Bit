// include comuni in libreria "GP_library"
#include "..\GP_library.h"


/*-----------------------------------------------------------*/
/* function FSMTINY_set_timer()                              */
/* Imposta una temporizzazione pari al valore del tick-timer */
/* più il valore ricevuto in ingresso                        */
/*-----------------------------------------------------------*/
void FSMTINY_set_timer(TINY_FSM* fsm,uint32_t timer_val)
  {
     fsm->timer = RTOS_get_tick_1ms() + timer_val;
  }

/*------------------------------------------------------------*/
/* function FSMTINY_check_EOT()                               */
/* Verifica la condizione di fine tempo della temporizzazione */
/* Restituisce TRUE se avvenuto fine tempo                    */
/*------------------------------------------------------------*/
BOOL FSMTINY_check_EOT(TINY_FSM* ptr)
  {
     if (RTOS_get_tick_1ms() <= ptr->timer)
       return FALSE;
     else
       return TRUE;
  }

void FSMTINY_init(TINY_FSM* ptr, uint8_t init_stat)
  {
     ptr->stat = init_stat;
     ptr->first = TRUE;
  }

void FSMTINY_set_state(TINY_FSM* ptr, uint8_t new_stat)
{
     ptr->first = TRUE;
     ptr->stat = new_stat;
}

BOOL FSMTINY_first(TINY_FSM* ptr)
{
    BOOL retval;
    retval = ptr->first;
    if (retval)
    {
        ptr->first = FALSE;
    }
    return retval;
}