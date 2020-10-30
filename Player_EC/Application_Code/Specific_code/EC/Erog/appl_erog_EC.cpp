/****************************************************************************/
/* File:           appl_erog.cpp                                            */
/* Data creazione: 11/05/2020                                               */
/* Creato da:      Ivan De Stefani                                          */
/* Funzione:       Modulo per la gestione degli stati di erogazione del     */
/*                       distributore di otoprotettori Argo EC Pro.         */
/* Changelog:                                                               */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
#include "player_EC.h"

// vedi se player tipo EC
#if MACHINE_TYPE == ARGO_EC

#define MODULE_APPL_EROG

// Stringhe costanti per stampe di debug
const char str_erog_init_start[] PROGMEM         = { "- Avvio inizializzazione appl_erog -" };
const char str_erog_init_end[] PROGMEM           = { "- Fine inizializzazione appl_erog -" };
const char str_erog_ev_sent[] PROGMEM            = { "Evento gestore erogazione: " };
const char str_erog_ev_scan[] PROGMEM            = { "EROG_EV_SCAN" };
const char str_erog_ev_scan_auth[] PROGMEM       = { "EROG_EV_SCAN_AUTH" };
const char str_erog_ev_bdg_timeout[] PROGMEM     = { "EROG_EV_BDG_TIMEOUT" };
const char str_erog_ev_bdg_unknown[] PROGMEM     = { "EROG_EV_BDG_UNKNOWN" };
const char str_erog_ev_bdg_ceased[] PROGMEM      = { "EROG_EV_BDG_CEASED" };
const char str_erog_ev_bdg_unauth[] PROGMEM      = { "EROG_EV_BDG_UNAUTH" };
const char str_erog_ev_bdg_outoftime[] PROGMEM   = { "EROG_EV_BDG_OUTOFTIME" };
const char str_erog_ev_bdg_ok[] PROGMEM          = { "EROG_EV_BDG_OK" };
const char str_erog_ev_unknown[] PROGMEM         = { "Evento sconosciuto!" };
const char str_erog_st_idle[] PROGMEM            = { "ST_EROG_IDLE" };
const char str_erog_st_start[] PROGMEM           = { "ST_EROG_START" };
const char str_erog_st_waiting_mano[] PROGMEM    = { "ST_EROG_WAITING_MANO" };
const char str_erog_st_waiting_t1[] PROGMEM      = { "ST_EROG_WAITING_T1" };
const char str_erog_st_waiting_t2[] PROGMEM      = { "ST_EROG_WAITING_T2" };
const char str_erog_st_erog_OK[] PROGMEM         = { "ST_EROG_OK" };
const char str_erog_st_erog_KO[] PROGMEM         = { "ST_EROG_KO" };
const char str_erog_OK[] PROGMEM                 = { "Erogazione OK!!" };
const char str_erog_KO[] PROGMEM                 = { "Erogazione KO ..." };

// Vettore di program memory per i nomi degli stati
const char* const erog_state_name[] PROGMEM =
{
     str_erog_st_idle,
     str_erog_st_start,
     str_erog_st_waiting_mano,
     str_erog_st_waiting_t1,
     str_erog_st_waiting_t2,
     str_erog_st_erog_OK,
     str_erog_st_erog_KO
};

// Macchina a stati di tipo complesso per la gestione dell'erogazione
static FSM_WORK FSM_Deliver;
static BOOL first_delivery_ko;
static BOOL flg_erog_start;
static BOOL flg_erog_stop;

// Prototipi funzione locali
static void  always_on_code(void);
static void gestione_conteggio_tappi(void);

uint16_t erog_sizeof_state_message(void)
  {
      return sizeof(erog_state_name);
  }

/***************************************/
/* erog_init()                         */
/*_____________________________________*/
/* Funzione di inizializzazione della  */
/* macchina a stati di tipo complesso  */
/* per la gestione dell'erogazione.    */
/***************************************/
void erog_init(void)
  {
     debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_erog_init_start);
     
     // Inizializzazione macchina della consegna
     fsm_init(&FSM_Deliver,
              ST_EROG_IDLE,
              (void *)erog_state_name,
              ST_EROG_KO,
              "ERG");
     flg_erog_start = FALSE;
     flg_erog_stop = FALSE;
     debug_message_timestamp_PGM(str_erog_init_end);
  }

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


/*****************************************/
/* erog_hand()                           */
/*_______________________________________*/
/* Funzione di gestione dell'erogazione. */
/*****************************************/
void erog_hand(void)
  {
     char erogbuff[80];

     switch (FSM_Deliver.state)
       {
          case ST_EROG_IDLE:
            if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_idle)))
              {
                 // IVAN : piccola modifica x fermare il motore
                 //        e' solo una prova ..
                 motor_send_event(MOT_EV_STOP);
              }
            else
              {
                 // vedi se richiesta di start
                 if (flg_erog_start)
                   {
                      flg_erog_start = FALSE;  
                      fsm_set_state(&FSM_Deliver, ST_EROG_START);
                   }
              }
            break;

          case ST_EROG_START:
            if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_start)))
              {
              }
            else
              {
                 fsm_set_state(&FSM_Deliver, ST_EROG_WAITING_MANO);
                 first_delivery_ko = FALSE;
                 app_globdata.tappi = 0;                 
              }
            break;

          case ST_EROG_WAITING_MANO:
            if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_waiting_mano)))
              {
              }
            else
              {
                 if (app_globdata.deliver_auth)
                   {
                      #ifdef DEBUG_VERSION
                      sprintf(erogbuff, "Tappi: %02d", app_globdata.tappi);
                      debug_print_timestamp(DBG_ALWAYS_ON, erogbuff);
                      #endif
               
                      switch (app_globdata.tappi)
                        {
                           case 0:
                             fsm_set_state(&FSM_Deliver, ST_EROG_WAITING_T1);
                             break;
                           case 1:
                             fsm_set_state(&FSM_Deliver, ST_EROG_WAITING_T2);
                             break;
                           default:
                             ;
                        }
                    }
                  motor_send_event(MOT_EV_START);      
              }
            break;

        case ST_EROG_WAITING_T1:
          fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_waiting_t1));
          // e se sono != 1 che facciamo ?
          // IDS: hai ragione, non ci avevo pensato! Ci vorrebbe uno switch che controlla il numero dei tappi
          if (app_globdata.tappi == 1)
            {               
               fsm_set_state(&FSM_Deliver, ST_EROG_WAITING_T2);
            }   
          break;

        case ST_EROG_WAITING_T2:
          fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_waiting_t2));
          if (app_globdata.tappi >= 2)
            {
               fsm_set_state(&FSM_Deliver, ST_EROG_OK);
            }
          break;

        case ST_EROG_OK:
          if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_erog_OK)))
            {
               fsm_set_timer(&FSM_Deliver, FSM_TIMER2, EROG_END_TIME);

               #ifdef DEBUG_VERSION
               debug_print_timestamp(erg_dbg, AVR_PGM_to_str(str_erog_OK));
               #endif
               
               motor_send_event(MOT_EV_STOP);
            }
          if (fsm_check_end_time(&FSM_Deliver, FSM_TIMER2))
            {
               fsm_set_state(&FSM_Deliver, ST_EROG_IDLE);
               app_globdata.tappi = 0;
            }
          break;

        case ST_EROG_KO:
          if (fsm_first_scan(&FSM_Deliver, AVR_PGM_to_str(str_erog_st_erog_KO)))
          {
               fsm_set_timer(&FSM_Deliver, FSM_TIMER2, EROG_END_TIME);

               #ifdef DEBUG_VERSION
                    debug_print_timestamp(erg_dbg, AVR_PGM_to_str(str_erog_KO));
               #endif

               motor_send_event(MOT_EV_STOP);
          }
          if (fsm_check_end_time(&FSM_Deliver, FSM_TIMER2))
          {
               app_globdata.tappi = 0;
          }
          break;

     default:
          ;
     }

     // Gestione del timeout per tutti i casi della macchina a stati.
     // Mettendolo fuori dallo switch ma permettendo un primo passaggio in
     // SUBST_EROG_START, siamo sicuri che il timer venga inizializzato
     // correttamente e non venga preso un timeout per errore.

     // questo codice va spostato nella always_on_code()
/*
     if (fsm_timer_hand(&FSM_Deliver, FSM_TIMER1, DELIVER_TIMEOUT))
     {
          if (!first_delivery_ko)
          {
               fsm_set_state(&FSM_Deliver, ST_EROG_KO);
               first_delivery_ko = TRUE;
          }
     }
     if (!deliver_auth && FSM_Deliver.state != ST_EROG_OK && FSM_Deliver.state != ST_EROG_KO && FSM_Deliver.state != ST_EROG_WAITING_MANO)
     {
          fsm_set_state(&FSM_Deliver, ST_EROG_WAITING_MANO);
     }
     */
     always_on_code();

  }

/*---------------------------------------------------------*/
/* function always_on_code()                               */
/* Gestisce quella porzione di codice che deve sempre      */
/* essere eseguita indipendentemente dallo stato della FSM */
/*---------------------------------------------------------*/
static void always_on_code(void)
  {
     // gestione dello stop
     if (flg_erog_stop)
       {
          flg_erog_stop = FALSE;
          // se non e' gia in IDLE mandala 
          if (FSM_Deliver.state != ST_EROG_IDLE)
            {
               fsm_set_state(&FSM_Deliver, ST_EROG_IDLE);
            }
       }

     // gestione conteggio tappi
     gestione_conteggio_tappi();
  }


/*--------------------------------------------------------*/
/* Inserire qui la gestione a livello del loop principale */
/* per il conteggio dei tappi                             */
/*--------------------------------------------------------*/
static void gestione_conteggio_tappi(void)
  {
     // vedi se evento da inresso veloce
     if (hi_speed_get_event())
       {
          app_globdata.tappi++;
       }
  }

/*****************************************/
/* erog_event_send()                     */
/*_______________________________________*/
/* Gestore dell'invio degli eventi alla  */
/* macchina per la gestione erogazione.  */
/*****************************************/
/*void erog_event_send(uint8_t EROG_EV_ID)
{
     // Stampa di debug di quale evento è stato inviato al gestore degli eventi per l'erogazione
     #ifdef DEBUG_VERSION
          debug_print_timestamp_title(erg_dbg, DEBUG_IDENT_L2, AVR_PGM_to_str(str_erog_ev_sent), erog_event_to_str(EROG_EV_ID));
     #endif

     switch (EROG_EV_ID)
     {
          case EROG_EV_BDG_OK:
               fsm_set_state(&FSM_Deliver, ST_EROG_START);             // Badge OK
               break;
          
          default:
               ;
     }

}*/

/************************************************/
/* erog_event_to_str()                          */
/*______________________________________________*/
/* Associa all'evento passato come parametro    */
/* la stringa di Program Memory corrispondente. */
/************************************************/
/*static char* erog_event_to_str(uint8_t event_ID)
{
     char buff[21];
     memset(buff, 0, sizeof(buff));

     switch (event_ID)
     {
          case EROG_EV_BDG_OK:
               strncpy(buff, AVR_PGM_to_str(str_erog_ev_bdg_ok), sizeof(buff) - 1);
               break;

          default:
               strncpy(buff, AVR_PGM_to_str(str_erog_ev_unknown), sizeof(buff) - 1);
               break;
     }

     return buff;
}*/

#undef MODULE_APPL_EROG
#endif