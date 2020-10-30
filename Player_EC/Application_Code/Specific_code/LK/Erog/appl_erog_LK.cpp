/****************************************************************************/
/* File:           appl_erog_LK.cpp                                         */
/* Data creazione: 30/10/2020                                               */
/* Creato da:      Ivan De Stefani                                          */
/* Funzione:       Modulo per la gestione degli stati di erogazione dell'   */
/*                 armadio Argo LK Pro.                                     */
/* Changelog:                                                               */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#include "player_EC.h"

#if MACHINE_TYPE == ARGO_LK

#define MODULE_APPL_EROG

const char str_st_erog_init[] PROGMEM        = { "ST_EROG_INIT" };
const char str_st_erog_idle[] PROGMEM        = { "ST_EROG_IDLE" };
const char str_st_erog_start[] PROGMEM       = { "ST_EROG_START" };
const char str_st_erog_select_mode[] PROGMEM = { "ST_EROG_SELECT_MODE" };
const char str_st_erog_prelievo[] PROGMEM    = { "ST_EROG_PRELIEVO" };
const char str_st_erog_reso[] PROGMEM        = { "ST_EROG_RESO" };
const char str_st_erog_refill[] PROGMEM      = { "ST_EROG_REFILL" };
const char str_st_erog_done[] PROGMEM        = { "ST_EROG_DONE" };
const char str_st_erog_error[] PROGMEM       = { "ST_EROG_ERROR" };
const char str_st_erog_fail[] PROGMEM        = { "ST_ERIG_FAIL" };

const char str_erog_init_start[] PROGMEM     = { "- Avvio inizializzazione appl_erog -" };
const char str_erog_init_end[] PROGMEM       = { "- Fine inizializzazione appl_erog -" };

const char* const str_erog_state_name[] PROGMEM = {
  str_st_erog_init,
  str_st_erog_idle,
  str_st_erog_start,
  str_st_erog_select_mode,
  str_st_erog_prelievo,
  str_st_erog_reso,
  str_st_erog_refill,
  str_st_erog_done,
  str_st_erog_error,
  str_st_erog_fail
};

static FSM_WORK FSM_Deliver;
static BOOL first_delivery_ko;
static BOOL flg_erog_start;
static BOOL flg_erog_stop;

void erog_init(void)
{
     debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_erog_init_start);

     fsm_init(&FSM_Deliver,
               ST_EROG_INIT,
               (void *) str_erog_state_name,
               ST_EROG_FAIL,
               (char *)"[ERG]");

     // Imposto i flag di start e stop erogazione a "falso"
     flg_erog_start = FALSE;
     flg_erog_stop = FALSE;

     debug_message_timestamp_PGM(str_erog_init_end);
}

void erog_start(void)
{
  flg_erog_start = TRUE;
}


void erog_hand(void)
{
  switch (FSM_Deliver.state)
  {
    case ST_EROG_INIT:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_init)))
      {

      }

      break;
    
    case ST_EROG_IDLE:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_idle)))
      {

      }

      break;
  
    case ST_EROG_START:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_start)))
      {

      }

      break;

      // Selezione di prelievo o reso
    case ST_EROG_SELECT_MODE:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_select_mode)))
      {

      }

      break;

    case ST_EROG_PRELIEVO:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_prelievo)))
      {

      }

      break;
  
    case ST_EROG_RESO:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_reso)))
      {

      }

      break;

    case ST_EROG_REFILL:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_refill)))
      {

      }

      break;

    case ST_EROG_DONE:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_done)))
      {

      }

      break;
  
    case ST_EROG_ERROR:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_error)))
      {

      }

      break;

    case ST_EROG_FAIL:
      if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_st_erog_fail)))
      {

      }

      break;
  
    default:
      break;
  }
}
#endif