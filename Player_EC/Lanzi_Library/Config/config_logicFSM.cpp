// include comune librerie LANZI
#include "../Lanzi_library.h"

// definizioni locali

//--------------------------------------------------------------------//
// enumerazione stati per lettura config da LAN
enum LAN_read_config_substate
  {
     LAN_READ_INIT = 0,    // condizione iniziale
     LAN_READ_WAIT,        // stato attesa comando
     LAN_READ_START,       // avvio operazioni
     LAN_READ_SEND_CMD,    // invio richiesta
     LAN_READ_WAIT_ANSWER, // attesa risposta
     LAN_READ_OK,          // lettura config completata con successo
     LAN_READ_ERROR,       // avvenuto errore
     LAN_READ_FAIL         // condizione di fail
  };
typedef LAN_read_config_substate LAN_READ_CONFIG_SUBSTAT;

// enumerazione cause di errore
enum config_FSM_error_code
  {
     CFG_ERR_NONE = 0,   // nessun errore
     CFG_ERR_WAIT_TIM,   // timeout attesa condizione ready
     CFG_ERR_ANSWER_TIM, // timeout attesa risposta
     CFG_ERR_ANSWR,      // errore nel contenuto della risposta
     CFG_ERR_INTERNAL    // errore interno
  };
typedef config_FSM_error_code CONFIG_FSM_ERR_CODE;

//--------------------------------------------------------------------//
// enumerazione stati per lettura config da SD
enum SD_read_config_substate
  {
     SD_READ_INIT = 0,    // condizione iniziale
     SD_READ_WAIT,        // stato attesa comando
     SD_READ_START,       // avvio operazioni
     SD_READ_SEND_CMD,    // invio richiesta
     SD_READ_WAIT_ANSWER, // attesa risposta
     SD_READ_OK,          // lettura config completata con successo
     SD_READ_ERROR,       // avvenuto errore
     SD_READ_FAIL         // condizione di fail
  };
typedef SD_read_config_substate SD_READ_CONFIG_SUBSTATE;

//--------------------------------------------------------------------//
// messaggi di stato per lettura config da LAN
static const char str_log_st_LAN_read_init[] PROGMEM        = { "ST_LAN_READ_INIT" };
static const char str_log_st_LAN_read_wait[] PROGMEM        = { "ST_LAN_READ_WAIT" };
static const char str_log_st_LAN_read_start[] PROGMEM       = { "ST_LAN_READ_START" };
static const char str_log_st_LAN_read_send_cmd[] PROGMEM    = { "ST_LAN_READ_SEND" };
static const char str_log_st_LAN_read_wait_answer[] PROGMEM = { "ST_LAN_READ_ANSWER" };
static const char str_log_st_LAN_read_ok[] PROGMEM          = { "ST_LAN_READ_OK" };
static const char str_log_st_LAN_read_error[] PROGMEM       = { "ST_LAN_READ_ERROR" };
static const char str_log_st_LAN_read_fail[] PROGMEM        = { "ST_LAN_READ_FAIL" };

// messaggi di stato per lettura config da SD
static const char str_log_st_SD_read_init[] PROGMEM        = { "ST_SD_READ_INIT" };
static const char str_log_st_SD_read_wait[] PROGMEM        = { "ST_SD_READ_WAIT" };
static const char str_log_st_SD_read_start[] PROGMEM       = { "ST_SD_READ_START" };
static const char str_log_st_SD_read_send_cmd[] PROGMEM    = { "ST_SD_READ_SEND" };
static const char str_log_st_SD_read_wait_answer[] PROGMEM = { "ST_SD_READ_ANSWER" };
static const char str_log_st_SD_read_ok[] PROGMEM          = { "ST_SD_READ_OK" };
static const char str_log_st_SD_read_error[] PROGMEM       = { "ST_SD_READ_ERROR" };
static const char str_log_st_SD_read_fail[] PROGMEM        = { "ST_SD_READ_FAIL" };

//--------------------------------------------------------------------//

// variabili locali

//=============================================================================//

/*-------------------------------------------------------------*/
/*     funzioni per gestire la lettura del config da LAN       */
/*-------------------------------------------------------------*/
/*--------------------------------------------------------*/
/* function config_LAN_read_init()                        */
/* Inizializza la FSM di lettura del config da LAN        */
/* (viene chiamata dalla "init" generale di applicazione) */
/*--------------------------------------------------------*/
void config_LAN_read_init(void)
  {
     FSMTINY_init(&config_work.LAN_FSM_read, LAN_READ_INIT);
  }

/*---------------------------------------------------------------*/
/* function config_LAN_read_init()                               */
/* Avvia le operazioni di lettura del confi da LAN               */
/* (viene richiamata dall FSM principale per avviare la lettura) */
/*---------------------------------------------------------------*/
void config_LAN_read_start(void)
  {
     config_work.LAN_read_start = TRUE;
  }

/*----------------------------------------*/
/* function config_LAN_read_hand()        */
/* Gestisce la lettura del config da LAN  */
/*----------------------------------------*/
void config_LAN_read_hand(void)
  {
     FSM_RETVAL esito;
     
     switch (config_work.LAN_FSM_read.stat)
       {
          case LAN_READ_INIT:
            if (FSMTINY_first(&config_work.LAN_FSM_read))
              {
                 config_debug_print_PGM(str_log_st_LAN_read_init);
                 FSMTINY_set_state(&config_work.LAN_FSM_read,LAN_READ_WAIT);
              }
            break;

          // attesa comando lettura
          case LAN_READ_WAIT:
            if (FSMTINY_first(&config_work.LAN_FSM_read))
              {
                 config_debug_print_PGM(str_log_st_LAN_read_wait);
              }
            else
              {
                 // vedi se arrivata richiesta lettura
                 if (config_work.LAN_read_start)
                   {
                      // azzera flag di richiesta e avvia operazioni
                      config_work.LAN_read_start = FALSE;
                      FSMTINY_set_state(&config_work.LAN_FSM_read,LAN_READ_START);
                   }
              }
            break;

          // verifica che il gestore del server sia "ready" ed eventualmente attendi
          case LAN_READ_START:
            if (FSMTINY_first(&config_work.LAN_FSM_read))
              {
                 config_debug_print_PGM(str_log_st_LAN_read_start);
                 // verifico che il gestore del server sia pronto a eseguire
                 esito = server_request_state();
                 switch (esito)
                   {
                      // se "READY" invia comando
                      case FSM_READY:
                        FSMTINY_set_state(&config_work.LAN_FSM_read, LAN_READ_SEND_CMD);
                        break;

                      // se in errore azzera l'errore e riprova
                      case FSM_ERROR:
                        server_reset_error();
                      // se in esecuzione attendi
                      case FSM_EXEC:
                        // gestisci i retry 
                        // nb : con LAN_retry = 0 tenta indefinitamente
                        if (config_work.LAN_retry)
                          {
                             config_work.LAN_retry--;
                             if (config_work.LAN_retry == 0)
                               {
                                  config_work.LAN_error_reason = CFG_ERR_WAIT_TIM;
                                  FSMTINY_set_state(&config_work.LAN_FSM_read, LAN_READ_ERROR);
                               } 
                             else
                               {
                                  // attiva tempo di attesa
                                  FSMTINY_set_timer(&config_work.LAN_FSM_read,1000L);
                               }
                           } 
                        break;

                      // se finito attendi
                      case FSM_DONE:
                        // attiva tempo di attesa
                        FSMTINY_set_timer(&config_work.LAN_FSM_read,500L);
                        break;

                      // avvenuta condizione di FAIL
                      case FSM_FAIL:
                        FSMTINY_set_state(&config_work.LAN_FSM_read, LAN_READ_FAIL);
                        break;

                      // gestione altri casi
                      default:
                        config_work.LAN_error_reason = CFG_ERR_INTERNAL;
                        FSMTINY_set_state(&config_work.LAN_FSM_read, LAN_READ_FAIL);;
                   }
              }
            else
              {
                 if (FSMTINY_check_EOT(&config_work.LAN_FSM_read))
                   {
                      // chiama lo stato corrente e quindi ripassa dalla first
                      FSMTINY_set_state(&config_work.LAN_FSM_read, LAN_READ_START);
                   }
              }
            break;

          // invia il comando di richiesta del config
          case LAN_READ_SEND_CMD:
            if (FSMTINY_first(&config_work.LAN_FSM_read))
              {
                 config_debug_print_PGM(str_log_st_LAN_read_send_cmd);
                 // avendo verificato che il gestore sia ready 
                 // la "server_request_send" restituisce sicuramente TRUE
                 (void)server_request_send(SRV_MSGID_RICHIESTA_CONFIG,"");
              }
            else
              {
                 FSMTINY_set_state(&config_work.LAN_FSM_read, LAN_READ_WAIT_ANSWER);
              }
            break;

          case LAN_READ_WAIT_ANSWER:
            if (FSMTINY_first(&config_work.LAN_FSM_read))
              {
                 config_debug_print_PGM(str_log_st_LAN_read_wait_answer);
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
                        FSMTINY_set_state(&config_work.LAN_FSM_read, LAN_READ_OK);
                        break; 
                      case FSM_ERROR:
                        config_work.LAN_error_reason = CFG_ERR_ANSWR;
                        FSMTINY_set_state(&config_work.LAN_FSM_read, LAN_READ_ERROR);
                        break;
                      case FSM_INVALID_STATE:
                        break;
                   }
              }
            break;

          case LAN_READ_OK:
            if (FSMTINY_first(&config_work.LAN_FSM_read))
              {
                 config_debug_print_PGM(str_log_st_LAN_read_ok);
              }
            break;

          case LAN_READ_ERROR:
            if (FSMTINY_first(&config_work.LAN_FSM_read))
              {
                 config_debug_print_PGM(str_log_st_LAN_read_error);
              }
            break;

          case LAN_READ_FAIL:
            if (FSMTINY_first(&config_work.LAN_FSM_read))
              {
                 config_debug_print_PGM(str_log_st_LAN_read_fail);
              }
            break;
       }
     return;
  }

/*-----------------------------------------------*/
/* function config_LAN_is_READY()                */
/* Restituisce TRUE se la FSM per la lettura del */
/* config e' pronta ad eseguire                  */
/*-----------------------------------------------*/
BOOL config_LAN_is_READY(void)
  {
     if (config_work.LAN_FSM_read.stat == LAN_READ_WAIT)
       return TRUE;
     else
       return FALSE;
  }


/*-----------------------------------------------*/
/* function config_LAN_is_OK()                   */
/* Restituisce TRUE se la lettura del config e'  */
/* andata a buon fine                            */
/*-----------------------------------------------*/
BOOL config_LAN_is_OK(void)
  {
     if (config_work.LAN_FSM_read.stat == LAN_READ_OK)
       return TRUE;
     else
       return FALSE;
  }

/*-----------------------------------------------*/
/* function config_LAN_is_ERROR()                */
/* Restituisce TRUE se la lettura del config ha  */
/* riscontrato un errore                         */
/*-----------------------------------------------*/
BOOL config_LAN_is_ERROR(void)
  {
     if (config_work.LAN_FSM_read.stat == LAN_READ_ERROR)
       return TRUE;
     else
       return FALSE;
  }

/*-----------------------------------------------*/
/* function config_LAN_is_FAIL()                 */
/* Restituisce TRUE se la lettura del config ha  */
/* riscontrato una condizione di fail            */
/*-----------------------------------------------*/
BOOL config_LAN_is_FAIL(void)
  {
     if (config_work.LAN_FSM_read.stat == LAN_READ_FAIL)
       return TRUE;
     else
       return FALSE;
  }

//=============================================================================//

/*--------------------------------------------------------*/
/* function config_SD_read_init()                         */
/* Inizializza la FSM di lettura del config da SD         */
/* (viene chiamata dalla "init" generale di applicazione) */
/*--------------------------------------------------------*/
void config_SD_read_init(void)
  {
     FSMTINY_init(&config_work.SD_FSM_read, SD_READ_INIT);
  }

/*---------------------------------------------------------------*/
/* function config_SD_read_init()                                */
/* Avvia le operazioni di lettura del confi da SD               */
/* (viene richiamata dall FSM principale per avviare la lettura) */
/*---------------------------------------------------------------*/
void config_SD_read_start(void) 
  {
     FSMTINY_init(&config_work.SD_FSM_read, SD_READ_WAIT);
  }

/*----------------------------------------*/
/* function config_SD_read_hand()        */
/* Gestisce la lettura del config da SD   */
/*----------------------------------------*/
void config_SD_read_hand(void)
  {
     FSM_RETVAL esito;

     switch (config_work.SD_FSM_read.stat)
       {
         case SD_READ_INIT:
           break;
         // attualmente questo stato passa automaticamente al successivo
         case SD_READ_WAIT:
           if (FSMTINY_first(&config_work.LAN_FSM_read))
             {
             }
           break;
         case SD_READ_SEND_CMD:
           if (FSMTINY_first(&config_work.LAN_FSM_read))
             {
             }
           break;
         case SD_READ_WAIT_ANSWER:
           if (FSMTINY_first(&config_work.LAN_FSM_read))
             {
             }
           break;
         case SD_READ_OK:
           if (FSMTINY_first(&config_work.LAN_FSM_read))
             {
             }
           break;
         case SD_READ_ERROR:
           if (FSMTINY_first(&config_work.LAN_FSM_read))
             {
             }
           break;
         case SD_READ_FAIL:
           if (FSMTINY_first(&config_work.LAN_FSM_read))
             {
             }
           break;
       }
  }

/*-----------------------------------------------*/
/* function config_SD_is_OK()                    */
/* Restituisce TRUE se la lettura del config e'  */
/* andata a buon fine                            */
/*-----------------------------------------------*/
BOOL config_SD_is_OK(void)
  {
     if (config_work.SD_FSM_read.stat == SD_READ_OK)
       return TRUE;
     else
       return FALSE;
  }

/*-----------------------------------------------*/
/* function config_SD_is_ERROR()                 */
/* Restituisce TRUE se la lettura del config ha  */
/* riscontrato un errore                         */
/*-----------------------------------------------*/
BOOL config_SD_is_ERROR(void)
  {
     if (config_work.SD_FSM_read.stat == SD_READ_ERROR)
       return TRUE;
     else
       return FALSE;
  }

/*-----------------------------------------------*/
/* function config_SD_is_FAIL()                  */
/* Restituisce TRUE se la lettura del config ha  */
/* riscontrato una condizione di fail            */
/*-----------------------------------------------*/
BOOL config_SD_is_FAIL(void)
  {
     if (config_work.SD_FSM_read.stat == SD_READ_FAIL)
       return TRUE;
     else
       return FALSE;
  }

//=============================================================================//

/*--------------------------------------------------------*/
/* function config_EEPROM_read_init()                     */
/* Inizializza la FSM di lettura del config da EEPROM     */
/* (viene chiamata dalla "init" generale di applicazione) */
/*--------------------------------------------------------*/
void config_EEPROM_read_init(void)
  {
  }

/*---------------------------------------------------------------*/
/* function config_EEPROM_read_init()                            */
/* Avvia le operazioni di lettura del confi da EEPROM            */
/* (viene richiamata dall FSM principale per avviare la lettura) */
/*---------------------------------------------------------------*/
void config_EEPROM_read_start(void) 
  {
  }

/*------------------------------------------*/
/* function config_EEPROM_read_hand()       */
/* Gestisce la lettura del config da EEPROM */
/*------------------------------------------*/
void config_EEPROM_read_hand(void)
  {
  }

/*-----------------------------------------------*/
/* function config_EEPROM_is_OK()                */
/* Restituisce TRUE se la lettura del config e'  */
/* andata a buon fine                            */
/*-----------------------------------------------*/
BOOL config_EEPROM_is_OK(void)
  {
  }

/*-----------------------------------------------*/
/* function config_EEPROM_is_ERROR()             */
/* Restituisce TRUE se la lettura del config ha  */
/* riscontrato un errore                         */
/*-----------------------------------------------*/
BOOL config_EEPROM_is_ERROR(void)
  {
  }

/*-----------------------------------------------*/
/* function config_EEPROM_is_FAIL()              */
/* Restituisce TRUE se la lettura del config ha  */
/* riscontrato una condizione di fail            */
/*-----------------------------------------------*/
BOOL config_EEPROM_is_FAIL(void)
  {
  }

//=============================================================================//
