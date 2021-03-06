/******************************************************************************/
/* File: FSM_template.cpp                                                     */
/* Data creazione: 26/03/2020  												  */
/* Creato da: Adriano Olivetti - Ivan De Stefani    						  */
/* Funzione: Template per implementazione strutture gestione macchine a stati */
/*																			  */
/* Changelog:																  */
/*																			  */
/*																			  */
/******************************************************************************/
// include comuni in libreria "GP_library"
#include "..\GP_library.h"

// prototipi funzione locali
static void fsm_print_state(FSM_WORK* fsm, uint8_t new_state);
static void fsm_get_stat_message(FSM_WORK* ptr, uint8_t state, char* buff, uint8_t size);

#ifdef ENV_AVR
void fsm_init(FSM_WORK* fsm, uint8_t initstat, void *state_name, uint8_t max_state, char* name)
#else
void fsm_init(FSM_WORK* fsm, uint8_t initstat, char** state_name, uint8_t max_state, char* name)
#endif
  {
     fsm->max_state = max_state;
     fsm->state_name = state_name;
     fsm->ena_print_chg = TRUE;
     fsm->ena_print_first = TRUE;
     fsm->ena_crlf_chg = TRUE;
     fsm->state = initstat;
     fsm->first = TRUE;
     strncpy(fsm->name, name, sizeof(fsm->name) - 1);
  }

void fsm_set_state(FSM_WORK *fsm, uint8_t new_state)
  {
 
    // salva lo stato corrente
    fsm->old_state = fsm->state;

    // visualizza messaggio stato
    if (fsm->ena_print_chg)
      {
         fsm_print_state(fsm, new_state);
      }

    // imposta in nuovo stato
    fsm->state = new_state;
    fsm->first = TRUE;
  }

void fsm_set_timer_light(FSMLIGHT_WORK* fsm, uint8_t timer_num)
  {
    switch (timer_num)
      {
         case FSM_TIMER1:
           fsm->delay_1 = 0;
           break;
         case FSM_TIMER2:
           fsm->delay_2 = 0;
           break;
         case FSM_TIMER3:
           fsm->delay_3 = 0;
           break;
         case FSM_TIMER4:
           fsm->delay_4 = 0;
           break;
      }

  }

/*---------------------------------------------*/
/* function fsm_set_timer()                    */
/* Setta uno dei timer della macchina a stati  */
/* In ingresso riceve :                        */
/* - il puntatore alla struttura della FSM     */
/* - il numero del timer da settare            */
/* - il valore di ritardo espresso in ms       */
/*---------------------------------------------*/
void fsm_set_timer(FSM_WORK* fsm, uint8_t timer_num,uint32_t tmr_delay)
  {
     switch (timer_num)
       {
          case FSM_TIMER1:
            fsm->delay_1 = RTOS_get_tick_1ms() + tmr_delay;
            break;
          case FSM_TIMER2:
            fsm->delay_2 = RTOS_get_tick_1ms() + tmr_delay;
            break;
          case FSM_TIMER3:
            fsm->delay_3 = RTOS_get_tick_1ms() + tmr_delay;
            break;
         case FSM_TIMER4:
           fsm->delay_4 = RTOS_get_tick_1ms() + tmr_delay;
           break;
       }
  }

/*---------------------------------------------------*/
/* function fsm_check_end_time()                     */
/* Verifica la condizione di fine tempo di uno       */
/* dei timer della FSM                               */
/* In ingresso riceve :                              */
/* - il puntatore alla struttura della FSM           */
/* - il numero del timer da verificare               */
/* In uscita restituisce TRUE se il tempo e' scaduto */
/*--------------------------------------------------*/
BOOL fsm_check_end_time(FSM_WORK* fsm, uint8_t timer_num)
  {
     BOOL retval = FALSE;
     uint32_t loc_ms;

     loc_ms = RTOS_get_tick_1ms();
     switch (timer_num)
       {
          case FSM_TIMER1:
            retval = (loc_ms <= fsm->delay_1) ? FALSE : TRUE;
            break;
          case FSM_TIMER2:
            retval = (loc_ms <= fsm->delay_2) ? FALSE : TRUE;
            break;
          case FSM_TIMER3:
            retval = (loc_ms <= fsm->delay_3) ? FALSE : TRUE;
            break;
          case FSM_TIMER4:
            retval = (loc_ms <= fsm->delay_4) ? FALSE : TRUE;
            break;
         default:
             ;
       }
    return retval;

  }

/* versione leggera basata su logica a tempo con base tempi (es: 10ms) */
BOOL fsm_timer_light_hand(FSMLIGHT_WORK* fsm, uint8_t timer_num, uint16_t delay)
  {
     BOOL retval;

     retval = FALSE;
     switch (timer_num)
       {
          case FSM_TIMER1:
            if (fsm->delay_1 < delay)
              {
                 fsm->delay_1++;
              }
            else
              {
                 retval = TRUE;
              }
            break;
       }
     return retval;
  }

  
void fsm_ena_print(FSM_WORK* fsm, BOOL chg, BOOL first,BOOL crlf)
  {
     fsm->ena_print_chg = chg;
     fsm->ena_print_first = first;
     fsm->ena_crlf_chg = crlf;
  }


static void fsm_print_state(FSM_WORK* fsm, uint8_t new_state)
  {
    char buffer[90];
    char old_state_name[MAX_MSG_STATE_SIZE];
    char new_state_name[MAX_MSG_STATE_SIZE];
    
    // recupera i nomi degli stati
    fsm_get_stat_message(fsm,fsm->old_state, old_state_name, sizeof(old_state_name));
    fsm_get_stat_message(fsm,new_state, new_state_name, sizeof(new_state_name));

    sprintf(buffer, "[%s] new=", fsm->name);
    strncat(buffer, new_state_name, sizeof(buffer) - 1);
    strncat(buffer, " old=", sizeof(buffer) - 1);
    strncat(buffer, old_state_name, sizeof(buffer) - 1);

    debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L1, buffer);

  }

// funzione gestione prima scansione in uno stato
// restituisce TRUE alla prima chiamata in uno specifico stato
// nb: azzera automaticamente il flag di primo passaggio
// visualizza un messaggio opzionale
BOOL fsm_first_scan(FSM_WORK* fsm,char * message)
{
    BOOL retval;

    retval = fsm->first;
    if (retval)
    {
        fsm->first = FALSE;
        if (fsm->ena_crlf_chg)
          {
             debug_print_ena(fsm->ena_print_first, (char *)"\n\r");
          }

        if (strlen(message) != 0)
          {
           char temp[10];
           temp[0] = '[';
           temp[1] = 0;
           strncat(temp, fsm->name, sizeof(temp) - 1);
           strncat(temp, "] [", sizeof(temp));
     
           debug_print_timestamp_title(fsm->ena_print_first, DEBUG_IDENT_NONE, temp,message);
           debug_print_ena(fsm->ena_print_first, "]");
          }
       
    }
    return retval;
}

static void fsm_get_stat_message(FSM_WORK *fsm, uint8_t state, char* buff, uint8_t size)
  {
     if (state <= fsm->max_state)
       {
          memset(buff, 0, size);
          char* state_message;
          size_t len;

          #ifdef ENV_AVR
          char **loc_erog_state_name;

          loc_erog_state_name = (char**)fsm->state_name;      
          state_message = (char*)pgm_read_word(&loc_erog_state_name[state]);
          len = strlen_P(state_message);
          #else   
          state_message = fsm->state_name[state];
          len = strlen(state_message);
          #endif
          if (len < MAX_MSG_STATE_SIZE)
            {
               strncpy(buff, AVR_PGM_to_str(state_message), size - 1);
            }
          else
            {
              strncpy(buff, "???", size - 1);
            }
           
       }
  }
