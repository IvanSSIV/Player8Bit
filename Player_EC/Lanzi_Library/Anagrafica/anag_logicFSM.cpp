// include comune librerie LANZI
#include "../Lanzi_library.h"

// definizioni locali

//--------------------------------------------------------------------//
// enumerazione stati per lettura anagrafica da LAN
enum LAN_read_anag_substate
  {
     LAN_READ_INIT = 0,    // condizione iniziale
     LAN_READ_START,  // attesa server OK
     LAN_READ_SEND_CMD,    // invio richiesta
     LAN_READ_WAIT_ANSWER, // attesa risposta
     LAN_READ_OK,          // lettura config completata con successo
     LAN_READ_ERROR,       // avvenuto errore
     LAN_READ_FAIL         // condizione di fail
  };
typedef LAN_read_anag_substate LAN_READ_ANAG_SUBSTAT;

// enumerazione cause di errore
enum config_FSM_error_code
  {
     CFG_ERR_NONE = 0,     // nessun errore
     CFG_ERR_WAIT_TIM,     // timeout attesa server ready
     CFG_ERR_ANSWER_TIM,   // timeout attesa risposta
     CFG_ERR_ANSWR         // errore nel contenuto della risposta
  };
typedef config_FSM_error_code CONFIG_FSM_ERR_CODE;

//--------------------------------------------------------------------//
// enumerazione stati per lettura anagrafica da SD
enum SD_read_anag_substate
  {
     SD_READ_INIT = 0,    // condizione iniziale
     SD_READ_WAIT,  // attesa server OK
     SD_READ_SEND_CMD,    // invio richiesta
     SD_READ_WAIT_ANSWER, // attesa risposta
     SD_READ_OK,          // lettura config completata con successo
     SD_READ_ERROR,       // avvenuto errore
     SD_READ_FAIL         // condizione di fail
  };
typedef SD_read_anag_substate SD_READ_ANAG_SUBSTATE;

//--------------------------------------------------------------------//

//--------------------------------------------------------------------//

// variabili locali
static TINY_FSM FSM_LAN_read; // FSM gestione lettura da LAN
static TINY_FSM FSM_SD_read;  // FSM gestione lettura da SD
static uint8_t  LAN_retry;    // conteggio retry per server ready
static uint8_t  LAN_error_reason; // ragione della condizion di errore

//=============================================================================//

/*-------------------------------------------------------------*/
/*     funzioni per gestire la lettura dell' anagrafica da LAN */
/*-------------------------------------------------------------*/
/*--------------------------------------------------------*/
/* function anag_LAN_read_init()                          */
/* Inizializza la FSM di lettura dell' anagrafica da LAN  */
/* (viene chiamata dalla "init" generale di applicazione) */
/*--------------------------------------------------------*/
void anag_LAN_read_init(void)
  {
     FSMTINY_init(&FSM_LAN_read, LAN_READ_INIT);
  }

/*---------------------------------------------------------------*/
/* function anag_LAN_read_init()                                 */
/* Avvia le operazioni di lettura dell'anagrafica da LAN         */
/* (viene richiamata dall FSM principale per avviare la lettura) */
/*---------------------------------------------------------------*/
void anag_LAN_read_start(void)
  {
     LAN_retry = 3;
     LAN_error_reason = CFG_ERR_NONE;
     FSMTINY_init(&FSM_LAN_read, LAN_READ_START);
  }

/*--------------------------------------------*/
/* function anag_LAN_read_hand()              */
/* Gestisce la lettura dell'anagrafica da LAN */
/*--------------------------------------------*/
void anag_LAN_read_hand(void)
  {
     FSM_RETVAL esito;
     
     switch (FSM_LAN_read.stat)
       {
          case LAN_READ_INIT:
            break;

          // verifica che il gestore del server sia "ready" ed eventualmente attendi
          case LAN_READ_START:
            if (FSMTINY_first(&FSM_LAN_read))
              {
                 // verifico che il gestore del server sia pronto a eseguire
                 if (server_request_state() == FSM_READY)
                   {
                      FSMTINY_set_state(&FSM_LAN_read, LAN_READ_SEND_CMD);
                   }
                 else
                   {
                      // gestisci i retry 
                      // nb : con LAN_retry = 0 tenta indefinitamente
                      if (LAN_retry)
                        {
                           LAN_retry--;
                           if (LAN_retry == 0)
                             {
                                LAN_error_reason = CFG_ERR_WAIT_TIM;
                                FSMTINY_set_state(&FSM_LAN_read, LAN_READ_ERROR);
                             } 
                        } 
                   }
              }
            else
              {
                if (FSMTINY_check_EOT(&FSM_LAN_read))
                  {
                     // chiama lo stato corrente e quindi ripassa dalla first
                     FSMTINY_set_state(&FSM_LAN_read, LAN_READ_START);
                  }
              }
            break;

          // invia il comando di richiesta del config
          case LAN_READ_SEND_CMD:
            if (FSMTINY_first(&FSM_LAN_read))
              {
                 // avendo verificato che il gestore sia ready 
                 // la "server_request_send" restituisce sicuramente TRUE
                 server_request_send(SRV_MSGID_RICHIESTA_CONFIG,"");
              }
            else
              {
                 FSMTINY_set_state(&FSM_LAN_read, LAN_READ_WAIT_ANSWER);
              }
            break;

          case LAN_READ_WAIT_ANSWER:
            if (FSMTINY_first(&FSM_LAN_read))
              {

              }
            else
              {
                 // legge lo stato del gestore del server
                 esito = server_request_state();
                 switch (esito)
                   {
                      case FSM_EXEC:
                        break;
                      case FSM_DONE:
                        FSMTINY_set_state(&FSM_LAN_read, LAN_READ_OK);
                        break; 
                      case FSM_ERROR:
                        FSMTINY_set_state(&FSM_LAN_read, LAN_READ_ERROR);
                        break;
                   }
              }
            break;

          case LAN_READ_OK:
            break;
          case LAN_READ_ERROR:
            break;
          case LAN_READ_FAIL:
            break;
       }
     return;
  }


/*---------------------------------------------------*/
/* function anag_LAN_is_OK()                         */
/* Restituisce TRUE se la lettura dell'anagrafica e' */
/* andata a buon fine                                */
/*---------------------------------------------------*/
BOOL anag_LAN_is_OK(void)
  {
     if (FSM_LAN_read.stat == LAN_READ_OK)
       return TRUE;
     else
       return FALSE;
  }

/*---------------------------------------------------*/
/* function anag_LAN_is_ERROR()                      */
/* Restituisce TRUE se la lettura dell'anagrafica ha */
/* riscontrato un errore                             */
/*---------------------------------------------------*/
BOOL anag_LAN_is_ERROR(void)
  {
     if (FSM_LAN_read.stat == LAN_READ_ERROR)
       return TRUE;
     else
       return FALSE;
  }

//=============================================================================//

/*--------------------------------------------------------*/
/* function anag_SD_read_init()                           */
/* Inizializza la FSM di lettura del anagrafica da SD     */
/* (viene chiamata dalla "init" generale di applicazione) */
/*--------------------------------------------------------*/
void anag_SD_read_init(void)
  {
     FSMTINY_init(&FSM_SD_read, SD_READ_INIT);
  }

/*---------------------------------------------------------------*/
/* function anag_SD_read_init()                                  */
/* Avvia le operazioni di lettura dell'anagrafica da SD          */
/* (viene richiamata dall FSM principale per avviare la lettura) */
/*---------------------------------------------------------------*/
void anag_SD_read_start(void) 
  {
     FSMTINY_init(&FSM_SD_read, SD_READ_WAIT);
  }

/*-------------------------------------------*/
/* function anag_SD_read_hand()              */
/* Gestisce la lettura dell'anagrafica da SD */
/*-------------------------------------------*/
void anag_SD_read_hand(void)
  {
  }

/*---------------------------------------------------*/
/* function anag_SD_is_OK()                          */
/* Restituisce TRUE se la lettura dell'anagrafica e' */
/* andata a buon fine                                */
/*---------------------------------------------------*/
BOOL anag_SD_is_OK(void)
  {
     if (FSM_SD_read.stat == SD_READ_OK)
       return TRUE;
     else
       return FALSE;
  }

/*---------------------------------------------------*/
/* function anag_SD_is_ERROR()                       */
/* Restituisce TRUE se la lettura dell'anagrafica ha */
/* riscontrato un errore                             */
/*---------------------------------------------------*/
BOOL anag_SD_is_ERROR(void)
  {
     if (FSM_SD_read.stat == SD_READ_ERROR)
       return TRUE;
     else
       return FALSE;
  }

//=============================================================================//

/*---------------------------------------------------------*/
/* function anag_EEPROM_read_init()                        */
/* Inizializza la FSM di lettura dell'anagrafica da EEPROM */
/* (viene chiamata dalla "init" generale di applicazione)  */
/*---------------------------------------------------------*/
void anag_EEPROM_read_init(void)
  {
  }

/*---------------------------------------------------------------*/
/* function anag_EEPROM_read_init()                              */
/* Avvia le operazioni di lettura dell'anagrafica da EEPROM      */
/* (viene richiamata dall FSM principale per avviare la lettura) */
/*---------------------------------------------------------------*/
void anag_EEPROM_read_start(void) 
  {
  }

/*------------------------------------------*/
/* function anag_EEPROM_read_hand()         */
/* Gestisce la lettura del config da EEPROM */
/*------------------------------------------*/
void anag_EEPROM_read_hand(void)
  {
  }

/*---------------------------------------------------*/
/* function anag_EEPROM_is_OK()                      */
/* Restituisce TRUE se la lettura dell'anagrafica e' */
/* andata a buon fine                                */
/*---------------------------------------------------*/
BOOL anag_EEPROM_is_OK(void)
  {
  }

/*---------------------------------------------------*/
/* function anag_EEPROM_is_ERROR()                   */
/* Restituisce TRUE se la lettura dell'anagrafica ha */
/* riscontrato un errore                             */
/*---------------------------------------------------*/
BOOL anag_EEPROM_is_ERROR(void)
  {
  }
