// Created by: Ivan De Stefani
// On:         06/11/2020
/***********************************************/
/* Modulo di gestione dell'erogazione per il   */
/* raccoglitore di guanti Argo GR Pro.         */
/***********************************************/

#include "player_EC.h"


#if MACHINE_TYPE == ARGO_GR

#define MODULE_APPL_EROG

#define EROG_ERROR_TIMEOUT  60000L
#define EROG_PRESENCE_TIME   3000L
#define EROG_STOP_MOTOR      5000L

const char str_erog_init_start[] PROGMEM               = { "- Avvio inizializzazione appl_erog -" };
const char str_erog_init_end[] PROGMEM                 = { "- Fine inizializzazione appl_erog -" };
const char str_erog_st_idle[] PROGMEM                  = { "ST_EROG_IDLE" };
const char str_erog_st_apri_sportello[] PROGMEM        = { "ST_EROG_APRI_SPORTELLO" };
const char str_erog_st_wait_sportello_chiuso[] PROGMEM = { "ST_EROG_WAIT_SPORTELLO_CHIUSO" };
const char str_erog_st_check_deposito[] PROGMEM        = { "ST_EROG_CHECK_DEPOSITO" };
const char str_erog_st_apri_botola[] PROGMEM           = { "ST_EROG_APRI_BOTOLA" };
const char str_erog_st_chiudi_botola[] PROGMEM         = { "ST_EROG_CHIUDI_BOTOLA" };
const char str_erog_st_check_caduta[] PROGMEM          = { "ST_EROG_CHECK_CADUTA" };
const char str_erog_st_erog_OK[] PROGMEM               = { "ST_EROG_OK" };
const char str_erog_st_erog_KO[] PROGMEM               = { "ST_EROG_KO" };
const char str_erog_st_erog_error[] PROGMEM            = { "ST_EROG_ERROR" };
const char str_erog_OK[] PROGMEM                       = { "Erogazione OK!!" };
const char str_erog_KO[] PROGMEM                       = { "Erogazione KO ..." };

const char* const erog_state_name[] PROGMEM = {
  str_erog_st_idle,
  str_erog_st_apri_sportello,
  str_erog_st_wait_sportello_chiuso,
  str_erog_st_check_deposito,
  str_erog_st_apri_botola,
  str_erog_st_chiudi_botola,
  str_erog_st_check_caduta,
  str_erog_st_erog_OK,
  str_erog_st_erog_KO
};

static FSM_WORK FSM_Deliver;
static BOOL first_delivery_ko;
static BOOL flg_erog_start;
static BOOL flg_erog_stop;
static uint8_t tentativi_consegna;

//================================================================//
// API di interfaccia tra il modulo "appl_erog" e la FSM_logica   //
//================================================================//
/*-------------------------------------------------------*/
/* function erog_is_ready()                              */
/* Restituisce TRUE se ci sono le condizioni per erogare */
/*-------------------------------------------------------*/
BOOL erog_is_ready(void)
  {
     return TRUE;
  }

/*------------------------------------------------------------*/
/* function erog_start()                                      */
/* Avvia il ciclo di erogazione prodotto                      */
/* Viene richiamata dalla FSM_logica per avviare l'erogazione */
/*------------------------------------------------------------*/
void erog_start(void)
  {
     flg_erog_start = TRUE;
  }

/*----------------------------------------------------------------------*/
/* function erog_stop()                                                 */
/* Stoppa il ciclo di erogazione prodotto (completa movimento in corso) */
/* Viene richiamata dalla FSM_logica per fermare l'erogazione           */
/*----------------------------------------------------------------------*/
void erog_stop(void)
  {
     flg_erog_stop = TRUE;
  }

/*---------------------------------------------------------*/
/* function erog_abort()                                   */
/* Interrompe immediatamente ogni operazione di erogazione */
/* (es : su apertura porta)                                */
/*---------------------------------------------------------*/
void erog_abort(void)
  {
     // in questa applicazione "dummy function"
  }

/*--------------------------------------------------------*/
/* function erog_is_OK()                                  */
/* Restituisce TRUE se l'erogazione e' andata a buon fine */
/* Viene richiamata dalla FSM principale per conoscere    */
/* l'esito dell' operazione                               */
/*--------------------------------------------------------*/
BOOL erog_is_OK(void)
  {
     // va gestita con opportuno codice
     return TRUE;
  }

/*--------------------------------------------------------*/
/* function erog_is_error()                               */
/* Restituisce TRUE se l'erogazione ha prodotto un errore */
/* Viene richiamata dalla FSM principale per conoscere    */
/* l'esito dell' operazione                               */
/*--------------------------------------------------------*/
BOOL erog_is_error(void)
  {
     // va gestita con opportuno codice
     return TRUE;
  }

/*-------------------------------------------------------*/
/* function erog_reset_error()                           */
/* Viene richiamata dalla FSM principale per azzera una  */
/* eventuale condizone di errore e rimettere la FSM_erog */
/* in condizione di operare                              */
/*-------------------------------------------------------*/
void erog_reset_error()
  {
  }

//=========================================================================//

void erog_init(void)
{
    debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_erog_init_start);
     
     // Inizializzazione macchina della consegna
     fsm_init(&FSM_Deliver,
              ST_EROG_IDLE,
              (void *)erog_state_name,
              ST_EROG_ERROR,
              "ERG");
     flg_erog_start = FALSE;
     flg_erog_stop = FALSE;
     debug_message_timestamp_PGM(str_erog_init_end);
}

void erog_hand(void)
{
  switch (FSM_Deliver.state)
  {
    case ST_EROG_IDLE:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_idle)))
    {

    }
    break;
    
    case ST_EROG_APRI_SPORTELLO:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_apri_sportello)))
      {
        fsm_set_timer(&FSM_Deliver, FSM_TIMER1, 700L);
        OUT_SERR = TRUE;
        tentativi_consegna = 0;
      }
      if (fsm_check_end_time(&FSM_Deliver, FSM_TIMER1))
      {
        OUT_SERR = FALSE;
        fsm_set_state(&FSM_Deliver, ST_EROG_WAIT_SPORTELLO_CHIUSO);
      }
     break;
    
    case ST_EROG_WAIT_SPORTELLO_CHIUSO:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_wait_sportello_chiuso)))
    {
      fsm_set_timer(&FSM_Deliver, FSM_TIMER2, EROG_ERROR_TIMEOUT);
    }
    if (fsm_check_end_time(&FSM_Deliver, FSM_TIMER2) &&
        !IN_SERR)
    {
      fsm_set_state(&FSM_Deliver, ST_EROG_KO);
    }

    if (IN_SERR)
    {
      fsm_set_state(&FSM_Deliver, ST_EROG_CHECK_DEPOSITO);
    }
    break;
    
    case ST_EROG_CHECK_DEPOSITO:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_check_deposito)))
    {
      fsm_set_timer(&FSM_Deliver, FSM_TIMER1, EROG_PRESENCE_TIME);
    }
    // Dopo il minimo tempo per il deposito, controlla se le fotocellule vedono un prodotto
    if (fsm_check_end_time(&FSM_Deliver, FSM_TIMER1))
    {
      if (IN_PRESENZA)
        fsm_set_state(&FSM_Deliver, ST_EROG_KO);
      else if (!IN_PRESENZA)
        fsm_set_state(&FSM_Deliver, ST_EROG_APRI_BOTOLA);
    }

    break;
    
    case ST_EROG_APRI_BOTOLA:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_apri_botola)))
    {
      fsm_set_timer(&FSM_Deliver, FSM_TIMER3, EROG_STOP_MOTOR);
      motor_send_event(MOT_EV_START_FWD);
    }
    // Se motore bloccato, vado in stato di errore
    if (motor_get_status()->FSM_state == ST_MOT_BLOCC)
    {
      fsm_set_state(&FSM_Deliver, ST_EROG_ERROR);
    }
    // Se passati i 5 secondi, chiudo la botola
    if (fsm_check_end_time(&FSM_Deliver, FSM_TIMER3))
    {
      fsm_set_state(&FSM_Deliver, ST_EROG_CHIUDI_BOTOLA);
    }
    break;
    
    case ST_EROG_CHIUDI_BOTOLA:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_chiudi_botola)))
    {
      motor_send_event(MOT_EV_START_REV);
      fsm_set_timer(&FSM_Deliver, FSM_TIMER3, EROG_STOP_MOTOR);
    }
    // Se motore bloccato, vado in stato di errore
    if (fsm_check_end_time(&FSM_Deliver, FSM_TIMER2))
    {
      if (motor_get_status()->FSM_state == ST_MOT_BLOCC)
      {
        fsm_set_state(&FSM_Deliver, ST_EROG_ERROR);
      }
    }
    // Se passati i 5 secondi, controllo se il prodotto è sceso
    if (fsm_check_end_time(&FSM_Deliver, FSM_TIMER3))
    {
        fsm_set_state(&FSM_Deliver, ST_EROG_CHECK_CADUTA);
    }
    break;
    
    case ST_EROG_CHECK_CADUTA:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_check_caduta)))
    {
      tentativi_consegna++;
      if (IN_PRESENZA)
      {
        fsm_set_state(&FSM_Deliver, ST_EROG_OK);
      }
      else
      {
        if (tentativi_consegna < 5)
        {
          fsm_set_state(&FSM_Deliver, ST_EROG_APRI_BOTOLA);
        }
        else
        {
          fsm_set_state(&FSM_Deliver, ST_EROG_KO);
        }
      }
    }
    break;
    
    case ST_EROG_OK:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_erog_OK)))
    {

    }
    break;
    
    case ST_EROG_KO:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_erog_KO)))
    {

    }
    break;
  
    case ST_EROG_ERROR:
    if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_erog_error)))
    {

    }
    break;

    default:
    break;
  }
}

#undef MODULE_APPL_EROG

#endif