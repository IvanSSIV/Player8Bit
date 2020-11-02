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

// enumerazione sotto stati dello stato "LOGIC_INIT"
enum logic_init_substate
  {
     ST_INIT_INIT = 0,        // condizione iniziale
     ST_INIT_CHECK_CONN,      // test connessione
     ST_INIT_WAIT_SERVICE,    // attesa completamento servizi
     ST_INIT_ACCESA,          // invia messaggio "ACCESA"
     ST_INIT_PCTIME,          // invia richiesta "PCTIME"
     ST_INIT_OK,              // inizializzazione OK
     ST_INIT_ERROR            // errore inizializzazione
};
typedef logic_init_substate LOGIC_INIT_SUBSTAT;

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
                   // modifica temporanea in fase di debug
                   // poi ripristinare
                   fsm_set_state(&FSM_Logica, ST_LOGIC_MAIN); 
                   //fsm_set_state(&FSM_Logica, ST_LOGIC_READ_CONFIG); 
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
                 case FSM_EXEC:
                   break;
                 // indipendentemente dall'esito passa allo stato successivo
                 case FSM_DONE:
                 case FSM_ERROR:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_SEND_OFFLINE);
                   break;
                 case FSM_FAIL:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
                 default:  
                   // gestione safe dei casi non previsti
                   global_data.FSM_fail_code = FAIL_LOGIC_INT_ERROR;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
              }
            break;

          // cerca e invia eventuali file offline
          case ST_LOGIC_SEND_OFFLINE:
            esito_stato = hand_ST_LOGIC_SEND_OFFLINE();
            switch (esito_stato)
              {
                 case FSM_EXEC:
                   break;
                 case FSM_DONE:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_MAIN);
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
                 default:  
                   // gestione safe dei casi non previsti
                   global_data.FSM_fail_code = FAIL_LOGIC_INT_ERROR;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
              }
            break;

          // Gestisce le decisioni sulle operazioni da compiere
          // in funzione di eventuali condizioni di errori
          // Viene richiamata dopo una accensione o dopo ogni 
          // lettura del PC_time e operazioni annesse
          case ST_LOGIC_MAIN:
            esito_stato = hand_ST_LOGIC_MAIN();
            switch (esito_stato)
              {
                 case FSM_DONE:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_BADGE);
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_EXEC:
                   break;
                 case FSM_FAIL:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
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
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
                 default: 
                   // gestione safe in caso di richiamo accidentale
                   global_data.FSM_fail_code = FAIL_LOGIC_INT_ERROR;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
              }
            break;

          // Gestisce l'attesa del passaggio badge
          // Esce per avvenuta lettura badge o scaduto tempo per richiesta di PCtime
          case ST_LOGIC_BADGE:
            esito_stato = hand_ST_LOGIC_BADGE();
            switch (esito_stato)
              {
                 case FSM_EXEC:
                   break;
                 case FSM_DONE:
                   // vedi se avvenuta lettura barcode
                   if (global_data.flg_barcode_read)
                     {
                        main_logic_state new_stat;
                        if (global_data.flg_LAN_OK)
                          {
                             new_stat = ST_LOGIC_HAND_ONLINE;
                          }
                        else
                          {
                             new_stat = ST_LOGIC_HAND_OFFLINE;
                          }
                        // modifica temporanea (da eliminare)
                        new_stat = ST_LOGIC_BADGE;
                        // seleziona opportuno stato in funzione della 
                        // condizione di connessione attiva o meno
                        fsm_set_state(&FSM_Logica, new_stat);
                     }
                   // vedi se richiesta PCtime
                   else if (global_data.flg_read_PCtime_req)
                     {
                        fsm_set_state(&FSM_Logica, ST_LOGIC_PCTIME);
                     }
                   break;
                 case FSM_ERROR:
                   break;
                 case FSM_FAIL:
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
                   break;
                 default: 
                   // gestione safe in caso di richiamo accidentale
                   global_data.FSM_fail_code = FAIL_LOGIC_INT_ERROR;
                   fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
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
// messaggi di debug dei sottostati dello stato "INIT"
const char str_log_st_init_sub_INIT[]         PROGMEM = { "init->INIT" };
const char str_log_st_init_sub_CHECK_CONN[]   PROGMEM = { "init->CHECK_CONN" };
const char str_log_st_init_sub_WAIT_SERVICE[] PROGMEM = { "init->WAIT_SERVICE" };
const char str_log_st_init_sub_ACCESA[]       PROGMEM = { "init->ACCESA" };
const char str_log_st_init_sub_PCTIME[]       PROGMEM = { "init->PCTIME" };
const char str_log_st_init_sub_OK[]           PROGMEM = { "init->OK" };
const char str_log_st_init_sub_ERROR[]        PROGMEM = { "init->ERROR" };
static FSM_RETVAL hand_ST_LOGIC_INIT(void)
  {
     static TINY_FSM init_FSM;
     static FSM_RETVAL retval;
     FSM_RETVAL esito;

     debug_message_enable(TRUE,2);

     retval = FSM_NOT_INIT;
     // vedi se prima scansione nello stato
     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_init)))
       {
          // inserire qui eventuali notifiche su display

          FSMTINY_init(&init_FSM, ST_INIT_INIT);

       }
     else
       {
          switch (init_FSM.stat)
            {
               // condizione iniziale
               case ST_INIT_INIT:
                 if (FSMTINY_first(&init_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_init_sub_INIT);
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
                           FSMTINY_set_state(&init_FSM,ST_INIT_CHECK_CONN);
                        } 
                   }
                 break;

               // verifica la connessione   
               case ST_INIT_CHECK_CONN:
                 if (FSMTINY_first(&init_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_init_sub_CHECK_CONN);
                   }
                 else
                   {
                      // verifica connessione
                      // nb: la "server_LAN_connection" da FALSE se la LAN e' in fail
                      if (server_LAN_connection())
                        {
                           global_data.flg_LAN_OK = TRUE; 
                        }
                      else
                        {
                           // inserire eventuali inzializzazioni addizionali
                           // 
                           global_data.flg_LAN_OK = FALSE;
                        }     
                      FSMTINY_set_state(&init_FSM,ST_INIT_WAIT_SERVICE);
                   }
                 break;

               // verifica avvio FSM dei vari servizi 
               case ST_INIT_WAIT_SERVICE:
                 if (FSMTINY_first(&init_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_init_sub_WAIT_SERVICE);
                   }
                 else
                   {
                      if (global_data.flg_LAN_OK)
                        {
                           FSMTINY_set_state(&init_FSM,ST_INIT_ACCESA);
                        }
                      else
                        {
                           FSMTINY_set_state(&init_FSM,ST_INIT_ACCESA);
                        }
                   }
                 break;

               case ST_INIT_ACCESA:
                 if (FSMTINY_first(&init_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_init_sub_ACCESA);
                      // avvio procedura per notificare accensione
                      // nb : essendo all'accensione si da per scontato 
                      //      che il gestore del server sia ready
                      // invio richiesta al gestore del server
                      server_request_send(SRV_MSGID_ACCESA, "");
                   }
                 else
                   {
                      // verifico che il gestore del server sia pronto a eseguire
                      // ovvero attendo il completamento del precedente comando
                      esito = server_request_state();
                      if (esito == FSM_READY)
                        {
                           FSMTINY_set_state(&init_FSM,ST_INIT_PCTIME);
                        }
                   }
                 break;

               case ST_INIT_PCTIME:
                 if (FSMTINY_first(&init_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_init_sub_PCTIME);
                      // invia la richiesta di PCtime
                      server_request_send(SRV_MSGID_PCTIME, "");
                   }
                 else
                   {
                      // verifico che il gestore del server sia pronto a eseguire
                      // ovvero attendo il completamento del precedente comando
                      esito = server_request_state();
                      if (esito == FSM_READY)
                        {
                           FSMTINY_set_state(&init_FSM,ST_INIT_OK);
                        }                      
                   }
                 break;

               case ST_INIT_OK:
                 if (FSMTINY_first(&init_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_init_sub_OK);
                   }
                 retval = FSM_DONE;
                 break;

               case ST_INIT_ERROR:
                 if (FSMTINY_first(&init_FSM))
                   {
                      // visualizza info debug
                      debug_message_timestamp_PGM(str_log_st_init_sub_ERROR);
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
// messaggi di debug dei sottostati dello stato "READ_CONFIG"
const char str_log_st_readconf_sub_INIT[]       PROGMEM = { "read_config->INIT" };
const char str_log_st_readconf_sub_GET_LAN[]    PROGMEM = { "read_config->GET FROM LAN" };
const char str_log_st_readconf_sub_GET_SD[]     PROGMEM = { "read_config->GET FROM SD" };
const char str_log_st_readconf_sub_GET_EEPROM[] PROGMEM = { "read_config->GET FROM EEPROM" };
const char str_log_st_readconf_sub_DONE[]       PROGMEM = { "read_config->DONE" };
const char str_log_st_readconf_sub_ERROR[]      PROGMEM = { "read_config->ERROR" };

static FSM_RETVAL hand_ST_LOGIC_READ_CONFIG(void)
  {
     static TINY_FSM config_read_FSM;
     FSM_RETVAL retval;

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

/*----------------------------------------------------------*/
/* function hand_ST_LOGIC_READ_ANAG()                       */
/* Gestisce la lettura dell'anagrafica dei badge abilitati  */
/* dal server o SD                                          */
/*----------------------------------------------------------*/
// messaggi di debug dei sottostati dello stato "READ_ANAG"
const char str_log_st_readanag_sub_INIT[]       PROGMEM = { "read_anagr->INIT" };
const char str_log_st_readanag_sub_GET_LAN[]    PROGMEM = { "read_anagr->GET FROM LAN" };
const char str_log_st_readanag_sub_GET_SD[]     PROGMEM = { "read_anagr->GET FROM SD" };
const char str_log_st_readanag_sub_GET_EEPROM[] PROGMEM = { "read_anagr->GET FROM EEPROM" };
const char str_log_st_readanag_sub_DONE[]       PROGMEM = { "read_anagr->DONE" };
const char str_log_st_readanag_sub_ERROR[]      PROGMEM = { "read_anagr->ERROR" };

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

/*-------------------------------------------------------*/
/* function hand_ST_LOGIC_PCTIME()                       */
/* Richiesta di data e ora aggiornati al server          */
/* con cadenza tipica di 60sec (programmabile da condig) */
/*-------------------------------------------------------*/
// enumerazione sotto stati dello stato "LOGIC_PCTIME"
enum logic_pctime_substate
  {
     ST_PCTIME_INIT = 0,    // condizione iniziale
     ST_PCTIME_WAIT_READY,  // attesa completamento servizi
     ST_PCTIME_CHECK_CONN,  // test connessione
     ST_PCTIME_SEND,        // invia richiesta "PCTIME"
     ST_PCTIME_DONE,        // lettura OK
     ST_PCTIME_ERROR        // errore lettura
  };
typedef logic_pctime_substate LOGIC_PCTIME_SUBSTAT;

// messaggi di debug dei sottostati dello stato "READ_CONFIG"
const char str_log_st_pctime_sub_INIT[]       PROGMEM = { "read_pctime->INIT" };
const char str_log_st_pctime_sub_WAIT_READY[] PROGMEM = { "read_pctime->WAIT_READY" };
const char str_log_st_pctime_sub_CHECK_CONN[] PROGMEM = { "read_pctime->CHECK_CONNN" };
const char str_log_st_pctime_sub_SEND[]       PROGMEM = { "read_pctime->SEND" };
const char str_log_st_pctime_sub_DONE[]       PROGMEM = { "read_pctime->DONE" };
const char str_log_st_pctime_sub_ERROR[]      PROGMEM = { "read_pctime->ERROR" };

static FSM_RETVAL hand_ST_LOGIC_PCTIME(void)
  {
     FSM_RETVAL retval;
     static TINY_FSM PCTIME_read_FSM;
     FSM_RETVAL esito;

     retval = FSM_EXEC;
     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_pctime)))
       {
          FSMTINY_init(&PCTIME_read_FSM, ST_READ_ANAG_INIT);
       }
     else 
       {
          switch (PCTIME_read_FSM.stat)
            {
               // condizione iniziale
               case ST_PCTIME_INIT:  
                 if (FSMTINY_first(&PCTIME_read_FSM))
                   {
                      debug_message_timestamp_PGM(str_log_st_pctime_sub_INIT);
                   }
                 else
                   {
                      FSMTINY_set_state(&PCTIME_read_FSM,ST_PCTIME_WAIT_READY);
                   }
                 break;

               // attesa completamento operazioni
               case ST_PCTIME_WAIT_READY:
                 if (FSMTINY_first(&PCTIME_read_FSM))
                   {
                      debug_message_timestamp_PGM(str_log_st_pctime_sub_WAIT_READY);
                   }
                 else
                   {
                      // verifico che il gestore del server sia pronto a eseguire
                      // ovvero attendo il completamento del precedente comando
                      esito = server_request_state();
                      if (esito == FSM_READY)
                        {
                           FSMTINY_set_state(&PCTIME_read_FSM,ST_PCTIME_CHECK_CONN);
                        }                      
                   }
                 break;

               // test connessione LAN
               case ST_PCTIME_CHECK_CONN:
                 if (FSMTINY_first(&PCTIME_read_FSM))
                   {
                      debug_message_timestamp_PGM(str_log_st_pctime_sub_CHECK_CONN);
                   }
                 else
                   {
                      // verifica connessione
                      // nb: la "server_LAN_connection" da FALSE se la LAN e' in fail
                      if (server_LAN_connection())
                        {
                           global_data.flg_LAN_OK = TRUE; 
                        }
                      else
                        {
                           // inserire eventuali inzializzazioni addizionali
                           // 
                           global_data.flg_LAN_OK = FALSE;
                        }     
                      FSMTINY_set_state(&PCTIME_read_FSM,ST_PCTIME_SEND);
                   }
                 break;


               // invia richiesta "PCTIME"
               case ST_PCTIME_SEND:
                 if (FSMTINY_first(&PCTIME_read_FSM))
                   {
                      debug_message_timestamp_PGM(str_log_st_pctime_sub_SEND);
                      // invia la richiesta di PCtime
                      server_request_send(SRV_MSGID_PCTIME, "");
                   }
                 else
                   {
                      esito = server_request_state();
                      if (esito == FSM_READY)
                        {
                           FSMTINY_set_state(&PCTIME_read_FSM,ST_PCTIME_DONE);
                        }                      
                   }
                 break;
               // inizializzazione OK
               case ST_PCTIME_DONE:
                 if (FSMTINY_first(&PCTIME_read_FSM))
                   {
                      debug_message_timestamp_PGM(str_log_st_pctime_sub_DONE);
                   }
                 else
                   {
                      retval = FSM_DONE;
                   }
                 break;
               // errore inizializzazione
               case ST_PCTIME_ERROR:
                 if (FSMTINY_first(&PCTIME_read_FSM))
                   {
                      debug_message_timestamp_PGM(str_log_st_pctime_sub_ERROR);
                   }
                 else
                   {
                      retval = FSM_ERROR;
                   }
                 break;
            }
         }
     return retval;     
  }

#define PCTIME_REQUEST_RATE 60000L

/*-----------------------------------------------------------------*/
/* function hand_ST_LOGIC_MAIN()                                   */
/* Costituisce il punto centrale in cui vengono prese le decisioni */
/* della macchina a stati principale                               */
/*-----------------------------------------------------------------*/
static FSM_RETVAL hand_ST_LOGIC_MAIN(void)
  {
     FSM_RETVAL retval;

     retval = FSM_EXEC;
     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_main)))
       {
          // leggi dal modulo "logic_init" l'esito delle attivita di caricamento di config e anag
          //BOOL tmp;
          //tmp = get_config_anag_status();
          // appl_logic_data.flg_serv_config_OK = logic_get_config_status();
          //appl_logic_data.flg_serv_anag_OK = logic_get_anag_status();
       }
     else // Operazioni effettuate in "loop" nel main di applicazione
       {
          retval = FSM_DONE;
       }
     return retval;
  }

/************************************************************/
/* Questa funzione viene chiamata solo dalle macchine che   */
/* necessitano di un azzeramento della posizione (XL).      */
/************************************************************/
static FSM_RETVAL hand_ST_LOGIC_FIND_ZERO(void)
  {
     return FSM_DONE;
  }

/*-------------------------------------------------------*/
/* function hand_ST_LOGIC_SEND_OFFLINE()                 */
/* Invio dei file offline nel caso in cui siano presenti */
/* e venga verificata la presenza della connessione LAN  */
/* alla richiesta del PCTime                             */
/*-------------------------------------------------------*/
static FSM_RETVAL hand_ST_LOGIC_SEND_OFFLINE(void)
  {
     FSM_RETVAL retval;

     retval = FSM_EXEC;
     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_send_offline)))
       {

       }
     else
       {
          retval = FSM_DONE;
       }
     return retval;
  }

/*-------------------------------------------------------------------*/
/* function hand_ST_LOGIC_MAIN()                                     */
/* E' lo stato in cui il player e' in attesa del passaggio del badge */ 
/* Esce da questo stato ad un passaggio del badge o se e' trascorso  */
/* il tempo per la lettura del PCtime                                */
/*-------------------------------------------------------------------*/
static FSM_RETVAL hand_ST_LOGIC_BADGE(void)
  {
     static uint32_t PCtime_request_timer;
     FSM_RETVAL retval;

     retval = FSM_EXEC;

     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_badge)))
       {
          badge_ena_read();
          PCtime_request_timer = RTOS_get_tick_1ms();
       }
     else
       {
          // vedi se arrivato badge
          if (badge_present())
            {
               global_data.flg_barcode_read = TRUE;
               retval = FSM_DONE;
            }
         // temporizzazione per lettura PC_time
         else if ((RTOS_get_tick_1ms() - PCtime_request_timer) >= PCTIME_REQUEST_RATE)
           {
              // funzione che legge il PCtime
              global_data.flg_read_PCtime_req = TRUE;
              retval = FSM_DONE;
           }
       }
     return retval;
  }

/*-------------------------------------------------------------*/
/* function hand_ST_LOGIC_HAND_ONLINE()                        */
/* In caso di presenza di connessione al server, viene inviato */ 
/* il messaggio di richiesta autorizzazioni al prelievo        */
/* il tempo per la lettura del PCtime                          */
/*-------------------------------------------------------------*/
static FSM_RETVAL hand_ST_LOGIC_HAND_ONLINE(void)
  {
     FSM_RETVAL retval;

     retval = FSM_EXEC;
     if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_hand_online)))
       {

       }

     return retval;
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