#define MODULE_APPL_LOGICA

/****************************************************************************/
/* File:           appl_logica                                              */
/* Data creazione: 15/05/2020                                               */
/* Creato da:      Ivan De Stefani                                          */
/* Funzione:       Modulo di gestione della logica principale dell'Argo     */
/*               EC Pro.                                                    */
/*                                                                          */
/****************************************************************************/

#include "player_EC.h"

// definizioni locali

// enumerazione sotto stati dello stato "LOGIC_READ_CONFIG"
enum logic_read_config_substate
  {
     ST_READ_CONF_INIT = 0,        // condizione iniziale
     ST_READ_CONF_GET_FROM_LAN,    // leggi da LAN
     ST_READ_CONF_GET_FROM_SD,     // leggi da SD
     ST_READ_CONF_GET_FROM_EEPROM, // leggi da EEPROM/FLASH
     ST_READ_CONF_OK,              // lettura config completata con successo
     ST_READ_CONF_ERROR            // config errato o assente
};
typedef logic_read_config_substate LOGIC_READ_CONFIG_SUBSTAT;

// enumerazione sotto stati dello stato "LOGIC_READ_ANAG"
enum logic_read_anag_substate
  {
     ST_READ_ANAG_INIT = 0,        // condizione iniziale
     ST_READ_ANAG_GET_FROM_LAN,    // leggi da LAN
     ST_READ_ANAG_GET_FROM_SD,     // leggi da SD
     ST_READ_ANAG_GET_FROM_EEPROM, // leggi da EEPROM/FLASH
     ST_READ_ANAG_OK,              // lettura config completata con successo
     ST_READ_ANAG_ERROR            // config errato o assente
};
typedef logic_read_anag_substate LOGIC_READ_ANAG_SUBSTAT;


// Macchina a stati per la gestione della logica
FSM_WORK FSM_Logica;


// Stringhe in program memory per stampa di debug
static const char str_log_st_init[] PROGMEM                     = { "ST_LOGIC_INIT" };
static const char str_log_st_read_config[] PROGMEM              = { "ST_LOGIC_READ_CONFIG"};
static const char str_log_st_read_anag[] PROGMEM                = { "ST_LOGIC_READ_ANAG"};
static const char str_log_st_fill_stat[] PROGMEM                = { "ST_LOGIC_FILL_STATUS" };
static const char str_log_st_pctime[] PROGMEM                   = { "ST_LOGIC_PCTIME"};
static const char str_log_st_send_offline[] PROGMEM             = { "ST_LOGIC_SEND_OFFLINE" };
static const char str_log_st_main[] PROGMEM                     = { "ST_LOGIC_MAIN" };
static const char str_log_st_find_zero[] PROGMEM                = { "ST_LOGIC_FIND_ZERO"};
static const char str_log_st_badge[] PROGMEM                    = { "ST_LOGIC_BADGE" };
static const char str_log_st_hand_online[] PROGMEM              = { "ST_LOGIC_HAND_ONLINE" };
static const char str_log_st_hand_offline[] PROGMEM             = { "ST_LOGIC_HAND_OFFLINE" };
static const char str_log_st_erog[] PROGMEM                     = { "ST_LOGIC_EROG" };
static const char str_log_st_richiesta_autorizzazione[] PROGMEM = { "ST_LOGIC_RICHIESTA_AUTORIZZAZIONE" };
static const char str_log_st_init_error[] PROGMEM               = { "ST_LOGIC_INIT_ERROR"};
static const char str_log_st_error[] PROGMEM                    = { "ST_LOGIC_ERROR" };
static const char str_log_st_fail[] PROGMEM                     = { "ST_LOGIC_FAIL" };

static const char str_log_init_start[] PROGMEM                  = { "- Inizializzazione appl_logica... -" };
static const char str_log_init_end[] PROGMEM                    = { "- Fine inizializzazione appl_logica! -" };


// Vettore di program memory per i nomi degli stati 
const char* const state_name[] PROGMEM = {
   str_log_st_init,
   str_log_st_read_config,
   str_log_st_read_anag,
   str_log_st_fill_stat,
   str_log_st_pctime,
   str_log_st_send_offline,
   str_log_st_main,
   str_log_st_find_zero,
   str_log_st_badge,
   str_log_st_hand_online,
   str_log_st_hand_offline,
   str_log_st_erog,
   str_log_st_richiesta_autorizzazione,
   str_log_st_init_error,
   str_log_st_error,
   str_log_st_fail
};

// Prototipi di funzione locali
static FSM_RETVAL hand_ST_LOGIC_INIT(void);
static FSM_RETVAL hand_ST_LOGIC_MAIN(void);
static FSM_RETVAL hand_ST_LOGIC_FILL_STATUS(void);
static FSM_RETVAL hand_ST_LOGIC_READ_CONFIG(void);
static FSM_RETVAL hand_ST_LOGIC_READ_ANAG(void);
static FSM_RETVAL hand_ST_LOGIC_FIND_ZERO(void);
static FSM_RETVAL hand_ST_LOGIC_PCTIME(void);
static FSM_RETVAL hand_ST_LOGIC_SEND_OFFLINE(void);
static FSM_RETVAL hand_ST_LOGIC_BADGE(void);
static FSM_RETVAL hand_ST_LOGIC_HAND_ONLINE(void);
static FSM_RETVAL hand_ST_LOGIC_HAND_OFFLINE(void);
static FSM_RETVAL hand_ST_LOGIC_EROG(void);
static FSM_RETVAL hand_ST_LOGIC_RICHIESTA_AUTORIZZAZIONE(void);
static FSM_RETVAL hand_ST_LOGIC_INIT_ERROR(void);
static FSM_RETVAL hand_ST_LOGIC_ERROR(void);
static void hand_ST_LOGIC_FAIL(void);
 

/***************************************/
/* logic_init()                        */
/* Inizializzazione della macchina a   */
/* stati per la gestione della logica. */
/***************************************/
void logic_init(void)
  {
     debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_log_init_start);

     fsm_init(&FSM_Logica, 
              ST_LOGIC_INIT,
              (void*)state_name,
              ST_LOGIC_FAIL,
              "LGC");

     fsm_ena_print(&FSM_Logica, TRUE, TRUE, TRUE);

     global_data.flg_boot = TRUE; // segnala primo passaggio dopo reset
     global_data.dbg_ena_ST_LOGIC_READ_CONFIG = TRUE;
   
     debug_message_timestamp_PGM(str_log_init_end);
  }

/***************************************/
/* logic_main()                        */
/* Gestione della logica principale.   */
/***************************************/
void logic_main(void)
  {
     FSM_RETVAL esito_stato;
     switch (FSM_Logica.state)
       {
          // effetta le inzializzazionei globali e 
          // le verifiche di guasti hw non recuperabili
          case ST_LOGIC_INIT:
            esito_stato = hand_ST_LOGIC_INIT();
            switch (esito_stato)
              {
                 // (stati transitori) non fare nulla
                 case FSM_NOT_INIT:
                 case FSM_EXEC:
                   break;   
                 // termine regolare
                 case FSM_DONE:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_READ_CONFIG); 
                   break;

                 // la "INIT" restituisce FAIL se c'e un problema HW
                 // i vari flag in global_data indicano la causa specifica
                 case FSM_FAIL:
                   global_data.FSM_fail_code = FAIL_LOGIC_HW;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;

                 // gestione dei casi non previsti
                 // nb: attualmente la INIT non prevede il caso ERROR
                 case FSM_ERROR:
                 default:
                   global_data.FSM_fail_code = FAIL_LOGIC_INT_ERROR;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
              }
            break;

          // legge il config da varie fonti 
          case ST_LOGIC_READ_CONFIG:
            esito_stato = hand_ST_LOGIC_READ_CONFIG();
            switch (esito_stato)
              {
                 // (stati transitori) non fare nulla
                 case FSM_NOT_INIT:
                 case FSM_EXEC:
                   break;   
                 // indipendentemente dall'esito passa allo stato successivo
                 // nb : in global_var.flg_config_OK vi e' l'esito dell'operazione
                 case FSM_DONE:
                 case FSM_ERROR:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_READ_ANAG);
                   break;
                 // gestione dei casi non previsti  
                 case FSM_FAIL:
                 default:
                   break;
              }
            break;

          // legge l'anagrafica da varie fonti
          case ST_LOGIC_READ_ANAG:
            esito_stato = hand_ST_LOGIC_READ_ANAG();
            switch (esito_stato)
              {
                 // indipendentemente dall'esito passa allo stato successivo
                 // nb : in global_var.flg_anag_OK vi e' l'esito dell'operazione
                 case FSM_DONE:
                 case FSM_ERROR:
                   switch (global_data.machine_id)
                     {
                        // x questo tipo di macchine salta lo stato 
                        //   di lettura celle settori
                        case ARGO_EC:
                        case ARGO_GR:
                        case ARGO_KD:
                          fsm_set_state(&FSM_Logica, ST_LOGIC_PCTIME);
                          break;
                        // gestione macchine con tabella di uso celle/settori
                        case ARGO_XL:
                        case ARGO_LK:
                          fsm_set_state(&FSM_Logica, ST_LOGIC_FILL_STATUS);
                          break;
                        default:
                          global_data.FSM_fail_code = FAIL_LOGIC_NO_CONFIG;
                          fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                     }
                   break;
                 case FSM_FAIL:
                   break;
              }
            break;

          // gestisce la lettura dalla tabella di uso di celle/settori
          // nelle macchine EC non e' implementata e passa oltre
          case ST_LOGIC_FILL_STATUS:
            esito_stato = hand_ST_LOGIC_FILL_STATUS();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_PCTIME);
                   break;
                 case FSM_ERROR:
                 case FSM_FAIL:
                 default:
                   // gestione safe dei casi non previsti
                   global_data.FSM_fail_code = FAIL_LOGIC_INT_ERROR;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
              }
            break;

          // legge la data e l'ora dal server
          case ST_LOGIC_PCTIME:
            esito_stato = hand_ST_LOGIC_PCTIME();
            switch (esito_stato)
              {
                 // indipendentemente dall'esito passa allo stato successivo
                 case FSM_DONE:
                 case FSM_ERROR:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_SEND_OFFLINE);
                   break;
                 case FSM_FAIL:
                 default: 
                   break;
              }
            break;

          // cerca e invia eventuali file offline
          case ST_LOGIC_SEND_OFFLINE:
            esito_stato = hand_ST_LOGIC_SEND_OFFLINE();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:
                 default:
                   break;
              }
            break;

          case ST_LOGIC_MAIN:
            esito_stato = hand_ST_LOGIC_MAIN();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:
                 default:  
                   // gestione safe dei casi non previsti
                   global_data.FSM_fail_code = FAIL_LOGIC_INT_ERROR;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
              }
            break;

          // gestisce la procedura di azzeramento del tamburo
          // sulle macchine tipo EC non e' implementata ne richiamata
          case ST_LOGIC_FIND_ZERO:
            esito_stato = hand_ST_LOGIC_FIND_ZERO();
            switch (esito_stato)
              {
                 case FSM_DONE:
                 case FSM_ERROR:
                 case FSM_FAIL:
                 default: 
                   // gestione safe in caso di richiamo accidentale
                   global_data.FSM_fail_code = FAIL_LOGIC_INT_ERROR;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
              }
            break;

          case ST_LOGIC_BADGE:
            esito_stato = hand_ST_LOGIC_BADGE();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:                 
                   break;
              }
            break;

          case ST_LOGIC_HAND_ONLINE:
            esito_stato = hand_ST_LOGIC_HAND_ONLINE();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:                 
                   break;
              }
            break;
          
          case ST_LOGIC_HAND_OFFLINE:
            esito_stato = hand_ST_LOGIC_HAND_OFFLINE();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:                 
                   break;
              }
            break;
          
          case ST_LOGIC_EROG:
            esito_stato = hand_ST_LOGIC_EROG();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:                 
                   break;
              }
            break;
          
          case ST_LOGIC_RICHIESTA_AUTORIZZAZIONE:
            esito_stato = hand_ST_LOGIC_RICHIESTA_AUTORIZZAZIONE();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:                 
                   break;
              }
            break;
          
          case ST_LOGIC_INIT_ERROR:
            esito_stato = hand_ST_LOGIC_INIT_ERROR();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:                 
                   break;
              }
            break;
          
          case ST_LOGIC_ERROR:
            esito_stato = hand_ST_LOGIC_ERROR();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:                 
                   break;
              }
            break;

          // gestione condizione di errore non recuperabile
          // non prevede condizione di uscita ne valore restituito
          // (esce solo con un reset del sistema)
          case ST_LOGIC_FAIL:
            hand_ST_LOGIC_FAIL();
            break;

          default:
            ;
       }
  }
//-------------------------------------------------------------------------------------------//
///////////////////////// FINE STATI DELLA MACCHINA PRINCIPALE ////////////////////////////////
//-------------------------------------------------------------------------------------------//

//===========================================================================//
//      Funzioni di implementazioni dei singoli stati                        //
//             della macchina a stati principale                             //
//===========================================================================//

//===========================================================================//
/*---------------------------------------------------------------------*/
/* function hand_ST_LOGIC_INIT()                                       */
/* Gestisce lo stato "ST_LOGIC_INIT" della macchina a stati principale */
/* - inizializza dati globali                                          */
/* - invia il messaggo "ACCESA"                                        */
/*---------------------------------------------------------------------*/
static FSM_RETVAL hand_ST_LOGIC_INIT(void)
  {
     FSM_RETVAL retval;

     retval = FSM_NOT_INIT;
     // vedi se prima scansione nello stato
     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_init)))
       {
          // inserire qui eventuali notifiche su display

          // verifica connessione
          // nb: la "server_LAN_connection" da FALSE se la LAN e' in fail
          if (server_LAN_connection())
            {
               global_data.flg_LAN_OK = TRUE; 
               // avvio procedura per notificare accensione
               // nb : essendo all'accensione si da per scontato 
               //      che il gestore del server sia ready
               // invio richiesta al gestore del server
               server_request_send(SRV_MSGID_ACCESA, "");
           }
          else
            {
               // inserire eventuali inzializzazioni addizionali
               // 
               global_data.flg_LAN_OK = FALSE;
            }     
       }
     else
       {
          // vedi se avvenuti fail hw durante inizializzazione 
          // a basso livello
          if (global_data.LAN_hw_fail || 
              global_data.SD_hw_fail || 
              global_data.RTC_hw_fail)
            {
               retval = FSM_FAIL;
            }
          else
            {
               retval = FSM_DONE;
            }
       }

     return retval;
   }

//===========================================================================//

/*--------------------------------------------------------------------------*/
/* function hand_ST_LOGIC_FILL_STATUS()                                     */
/* non implementata nelle macchine EC                                       */
/* Verifica dello stato di riempimento della macchina tramite l'esecuzione  */
/* delle seguenti operazioni:                                               */
/* 1- Apertura del file "RICARICA.DAT"                                      */
/* 2- Lettura degli ultimi 8 caratteri del file (chiave univoca per data    */
/*    e ora dell'ultima ricarica)                                           */
/* 3- Ricerca su scheda SD del file chiamato AAAAAAAA.DAT, contenente i     */
/* dati dei settori e celle svuotati                                        */
/* 4- Partendo da uno stato ipotetico di riempimento completo del           */
/*    distributore, si procede a suotare i settori e le celle indicati nel  */
/*    file per avere la situazione attuale della macchina                   */
/*--------------------------------------------------------------------------*/
static FSM_RETVAL hand_ST_LOGIC_FILL_STATUS(void)
  {
     return FSM_DONE;
  }


//===========================================================================//
// messaggi di debug dei sottostati dello stato "READ_CONFIG"
const char str_log_st_readconf_sub_INIT[]       PROGMEM = { "read_config->INIT" };
const char str_log_st_readconf_sub_GET_LAN[]    PROGMEM = { "read_config->GET FROM LAN" };
const char str_log_st_readconf_sub_GET_SD[]     PROGMEM = { "read_config->GET FROM SD" };
const char str_log_st_readconf_sub_GET_EEPROM[] PROGMEM = { "read_config->GET FROM EEPROM" };
const char str_log_st_readconf_sub_DONE[]       PROGMEM = { "read_config->DONE" };
const char str_log_st_readconf_sub_ERROR[]      PROGMEM = { "read_config->ERROR" };

/*-----------------------------------------------------------------------*/
/* function hand_ST_LOGIC_READ_CONFIG()                                  */
/* Gestisce la lettura e il caricamento in memoria della configurazione  */
/* della macchina . La lettura del CONFIG avviene secondo la seguente    */
/* priorita :                                                            */
/* - se e' presenta la connessione LAN viene scaricato dal server        */
/* - in caso alternativo viene caricato dalla SD                         */
/* - in caso il config su SD sia alterato viene ricuperato dalla EEPROM  */
/* Se nessuno in nessuno dei tre casi si e' trovato un config valido     */
/* la macchina va in blocco e passa in stato "---"                       */
/*-----------------------------------------------------------------------*/
static FSM_RETVAL hand_ST_LOGIC_READ_CONFIG(void)
  {
     static TINY_FSM config_read_FSM;
     static FSM_RETVAL retval;

     retval = FSM_NOT_INIT;
     // vedi se primo giro nello stato
     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_read_config)))
       {
          FSMTINY_init(&config_read_FSM, ST_READ_CONF_INIT);

          // abilita debug di stato
          debug_message_enable(global_data.dbg_ena_ST_LOGIC_READ_CONFIG,
                               DEBUG_IDENT_SUB_STATE);
       }
     else
       {
          switch (config_read_FSM.stat)
            {
               // inizializza operazioni di lettura del config
               case ST_READ_CONF_INIT:
                 if (FSMTINY_first(&config_read_FSM))
                   {   
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_readconf_sub_INIT);
                      // vedi se siamo ONline
                      if (global_data.flg_LAN_OK)
                        {
                           FSMTINY_set_state(&config_read_FSM, ST_READ_CONF_GET_FROM_LAN);
                        }
                      else
                        {
                           FSMTINY_set_state(&config_read_FSM, ST_READ_CONF_GET_FROM_SD);
                        }
                   }
                 break;

               // gestione lettura da LAN
               case ST_READ_CONF_GET_FROM_LAN:
                 if (FSMTINY_first(&config_read_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_readconf_sub_GET_LAN);

                      global_data.flg_serv_config_OK = FALSE;
                      // avvia richiesta lettura config
                      config_LAN_read_start();
                   }
                 else
                   {
                      // gestisci attesa operazioni
                      config_LAN_read_hand();
                      // vedi se lettura OK
                      if (config_LAN_is_OK())
                        {
                           global_data.flg_serv_config_OK = TRUE;
                           FSMTINY_set_state(&config_read_FSM, ST_READ_CONF_OK);
                        }
                      // vedi se errore
                      if (config_LAN_is_ERROR())
                        {
                           FSMTINY_set_state(&config_read_FSM, ST_READ_CONF_GET_FROM_SD);
                        }
                   }
                 break;

               // gestione lettura da SD
               case ST_READ_CONF_GET_FROM_SD:
                 if (FSMTINY_first(&config_read_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp(AVR_PGM_to_str(str_log_st_readconf_sub_GET_SD));
                      // avvia richiesta lettura config
                      config_SD_read_start();
                   }
                 else
                   {
                      // gestisci attesa operazioni
                      config_SD_read_hand();
                      // vedi se lettura OK
                      if (config_SD_is_OK())
                        {
                           FSMTINY_set_state(&config_read_FSM, ST_READ_CONF_OK);
                        }
                      // vedi se errore
                      if (config_SD_is_ERROR())
                        {
                           FSMTINY_set_state(&config_read_FSM, ST_READ_CONF_GET_FROM_EEPROM);
                        }
                   }
                 break;

               // gestione lettura da EEPROM
               case ST_READ_CONF_GET_FROM_EEPROM:
                 if (FSMTINY_first(&config_read_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp(AVR_PGM_to_str(str_log_st_readconf_sub_GET_EEPROM));
                      // avvia richiesta lettura config
                      config_EEPROM_read_start();
                   }
                 else
                   {
                      // gestisci attesa operazioni
                      config_EEPROM_read_hand();
                      // vedi se lettura OK
                      if (config_EEPROM_is_OK())
                        {
                           FSMTINY_set_state(&config_read_FSM, ST_READ_CONF_OK);
                        }
                      // vedi se errore
                      if (config_EEPROM_is_ERROR())
                        {
                           FSMTINY_set_state(&config_read_FSM, ST_READ_CONF_ERROR);
                        }
                   }
                 break;

               case ST_READ_CONF_OK:
                 if (FSMTINY_first(&config_read_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp(AVR_PGM_to_str(str_log_st_readconf_sub_DONE));
                   } 
                 retval = FSM_DONE;
                 break;

               case ST_READ_CONF_ERROR:
                 if (FSMTINY_first(&config_read_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp(AVR_PGM_to_str(str_log_st_readconf_sub_ERROR));
                   } 
                 retval = FSM_ERROR;
                 break;
               default:
                 retval = FSM_INVALID_STATE;
            }
       }
     return retval;
  }


//===========================================================================//
// messaggi di debug dei sottostati dello stato "READ_ANAG"
const char str_log_st_readanag_sub_INIT[]       PROGMEM = { "read_anagr->INIT" };
const char str_log_st_readanag_sub_GET_LAN[]    PROGMEM = { "read_anagr->GET FROM LAN" };
const char str_log_st_readanag_sub_GET_SD[]     PROGMEM = { "read_anagr->GET FROM SD" };
const char str_log_st_readanag_sub_GET_EEPROM[] PROGMEM = { "read_anagr->GET FROM EEPROM" };
const char str_log_st_readanag_sub_DONE[]       PROGMEM = { "read_anagr->DONE" };
const char str_log_st_readanag_sub_ERROR[]      PROGMEM = { "read_anagr->ERROR" };

/*----------------------------------------------------------*/
/* function hand_ST_LOGIC_READ_ANAG()                       */
/* Gestisce la lettura dell'anagrafica dei badge abilitati  */
/* dal server o SD                                          */
/*----------------------------------------------------------*/
static FSM_RETVAL hand_ST_LOGIC_READ_ANAG(void)
  {
     static TINY_FSM anag_read_FSM;
     static FSM_RETVAL retval;

     retval = FSM_NOT_INIT;

     // vedi se primo giro nello stato
     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_read_anag)))
       {
          FSMTINY_init(&anag_read_FSM, ST_READ_ANAG_INIT);
          
          // abilita debug di stato
          debug_message_enable(global_data.dbg_ena_ST_LOGIC_READ_ANAG,DEBUG_IDENT_SUB_STATE);
       }
     else
       {
          switch (anag_read_FSM.stat)
            {
               // inizializza operazioni di lettura anagrafica
               case ST_READ_ANAG_INIT:
                 if (FSMTINY_first(&anag_read_FSM))
                   {   
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_readanag_sub_INIT);
                      // vedi se siamo ONline
                      if (global_data.flg_LAN_OK)
                        {
                           FSMTINY_set_state(&anag_read_FSM, ST_READ_ANAG_GET_FROM_LAN);
                        }
                      else
                        {
                           FSMTINY_set_state(&anag_read_FSM, ST_READ_ANAG_GET_FROM_SD);
                        }
                   }
                 break;
               // gestione lettura da LAN  
               case ST_READ_ANAG_GET_FROM_LAN:
                  if (FSMTINY_first(&anag_read_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp(AVR_PGM_to_str(str_log_st_readanag_sub_GET_LAN));

                      global_data.flg_anag_OK = FALSE;
                      // avvia richiesta lettura config
                      anag_LAN_read_start();
                   }
                 else
                   {
                      // gestisci attesa operazioni
                      anag_LAN_read_hand();
                      // vedi se lettura OK
                      if (anag_LAN_is_OK())
                        {
                           global_data.flg_serv_config_OK = TRUE;
                           FSMTINY_set_state(&anag_read_FSM, ST_READ_ANAG_OK);
                        }
                      // vedi se errore
                      if (config_LAN_is_ERROR())
                        {
                           FSMTINY_set_state(&anag_read_FSM, ST_READ_ANAG_GET_FROM_SD);
                        }
                   }
                break;
               case ST_READ_ANAG_GET_FROM_SD:
                 break;
               case ST_READ_ANAG_GET_FROM_EEPROM:
                 break;
               case ST_READ_ANAG_OK:
                 break;
               case ST_READ_ANAG_ERROR:
                 break;
               default:
                 ;
            }
       }
  }

// Richiesta di data e ora aggiornati al server, con cadenza ogni 60 s
static FSM_RETVAL hand_ST_LOGIC_PCTIME(void)
  {
     
  }

#define PCTIME_REQUEST_RATE 60000L

static FSM_RETVAL hand_ST_LOGIC_MAIN(void)
  {
   static uint32_t PCtime_request_timer;

   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_main)))
     {
       // leggi dal modulo "logic_init" l'esito delle attivita di caricamento di config e anag
       //BOOL tmp;
       //tmp = get_config_anag_status();
      // appl_logic_data.flg_serv_config_OK = logic_get_config_status();
       //appl_logic_data.flg_serv_anag_OK = logic_get_anag_status();
       badge_ena_read();
       PCtime_request_timer = RTOS_get_tick_1ms();
     }
   else // Operazioni effettuate in "loop" nel main di applicazione
     {
       // vedi se arrivato badge
       if (badge_present())
         {
           //badge_get_last_reading(appl_logic_data.last_read_badge, sizeof(appl_logic_data.last_read_badge) - 1);
           // inserire logica di gestione badge
           // bla,bla,bla e se ok chiamo erog
         }
       // temporizzazione per lettura PC_time
       if ((RTOS_get_tick_1ms() - PCtime_request_timer) >= PCTIME_REQUEST_RATE)
         {
            // funzione che legge il PCtime
           // se OK chiamo la send_offline
           PCtime_request_timer = RTOS_get_tick_1ms();
         }
     }
}

/************************************************************/
/* Questa funzione viene chiamata solo dalle macchine che   */
/* necessitano di un azzeramento della posizione (XL).      */
/************************************************************/

static FSM_RETVAL hand_ST_LOGIC_FIND_ZERO(void)
{

}

// Invio dei file offline nel caso in cui siano presenti e venga verificata
// la presenza della connessione LAN alla richiesta del PCTime
static FSM_RETVAL hand_ST_LOGIC_SEND_OFFLINE(void)
{
   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_send_offline)))
   {

   }
}

// Gestione della lettura del badge

static FSM_RETVAL hand_ST_LOGIC_BADGE(void)
{
   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_badge)))
   {

   }
}

// In caso di presenza di connessione al server, viene inviato il messaggio di richiesta autorizzazioni
// al prelievo
static FSM_RETVAL hand_ST_LOGIC_HAND_ONLINE(void)
{
   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_hand_online)))
   {

   }
}

// In caso di mancata connessione al server, vengono letti i file sulla SD per determinare
// le autorizzazioni al prelievo da parte del badge scansionato
static FSM_RETVAL hand_ST_LOGIC_HAND_OFFLINE(void)
{
   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_hand_offline)))
   {

   }
}

/**************************************************************/
/* Porzione di codice che gestisce l'erogazione del prodotto; */
/* è strettamente dipendente dal tipo di distributore per     */
/* cui viene compilato il firmware.                           */
/**************************************************************/

static FSM_RETVAL hand_ST_LOGIC_EROG(void)
{
   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_erog)))
   {

   }
}
// Richiesta di autorizzazione al prelievo per badge sconosciuto o non abilitato;
// valida per tutte le macchine della linea Argo Pro
static FSM_RETVAL hand_ST_LOGIC_RICHIESTA_AUTORIZZAZIONE(void)
{
   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_richiesta_autorizzazione)))
   {

   }
}

// Gestione delle casistiche di errore durante l'inizializzazione, per
// impedire il funzionamento della macchina nel caso in cui non siano
// presenti i parametri necessari
static FSM_RETVAL hand_ST_LOGIC_INIT_ERROR(void)
{

}

// Errore nella logica di gestione della macchina
static FSM_RETVAL hand_ST_LOGIC_ERROR(void)
{
   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_error)))
   {

   }
}

// Failure hardware di uno dei componenti della scheda
static void hand_ST_LOGIC_FAIL(void)
{
   if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_fail)))
   {

   }
}
#undef MODULE_APPL_LOGICA