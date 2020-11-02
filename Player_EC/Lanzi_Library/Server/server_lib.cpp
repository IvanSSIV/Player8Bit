/****************************************************************************/
/* File: appl_server.cpp                                                    */
/* Data creazione: 07/04/2020                                                              */
/* Creato da: Ivan De Stefani                                                              */
/* Funzione: Gestione ad alto livello della logica del server.                   */
/*                                                                                               */
/* Changelog:                                                                                */
/*                                                                                               */
/*                                                                                               */
/****************************************************************************/
#define MODULE_APPL_SERVER

// include comune librerie LANZI
#include "../Lanzi_library.h"
#include "IPAddress.h"

extern IPAddress IP;
extern IPAddress GW;
extern IPAddress DNS;
extern IPAddress SM;

// Definizione della variabile per lo stato della FSM di gestione del server
static FSM_WORK Server_FSM;

IPAddress SERVER(0, 0, 0, 0);                                   // IP del server
uint32_t port;                                                       // Porta del server

// Stringhe costanti per debug e invio al server
const char str_server_init_start[] PROGMEM     = { "- Inizializzo logica server -" };
const char str_server_init_end[] PROGMEM       = { "- Fine inizializzazione logica server -" };
const char str_LAN_connect_OK[] PROGMEM        = { "Connessione con server stabilita ..." };
const char str_LAN_connect_KO[] PROGMEM        = { "Impossibile connettersi al server ..." };
const char str_srv_send_OK[] PROGMEM           = { "Invio messaggio riuscito" };
const char str_srv_send_KO[] PROGMEM           = { "Invio messaggio fallito" };
const char str_srv_request_send[] PROGMEM      = { "Server request send :" };
const char str_srv_answ_OK_dbg[] PROGMEM       = { "risposta server OK" };
const char str_srv_answ_data_err[] PROGMEM     = { "dati risposta errati" };
const char str_srv_sys_err[] PROGMEM           = { "errore di sistema " };
const char str_srv_invalid_msgid[] PROGMEM     = { "id messaggio non supportato" };
const char str_srv_impossible[] PROGMEM        = { "impossibile che succeda ...." };
const char str_srv_msg_in_carico[] PROGMEM     = { "*** messaggio preso in carico ***" };
const char str_srv_msg_non_accodato[] PROGMEM  = { "*** messaggio non accodato ***" };

const char str_srv_st_init[] PROGMEM           = { "SRV_ST_INIT" };
const char str_srv_st_idle[] PROGMEM           = { "SRV_ST_IDLE" };
const char str_srv_st_sending_only[] PROGMEM   = { "SRV_ST_SENDING_ONLY" };
const char str_srv_st_sending[] PROGMEM        = { "SRV_ST_SENDING" };
const char str_srv_st_waiting_answ[] PROGMEM   = { "SRV_ST_WAITING_ANSW" };
const char str_srv_st_done[] PROGMEM           = { "SRV_ST_DONE" };
const char str_srv_st_error[] PROGMEM          = { "SRV_ST_ERROR" };
const char str_srv_st_timeout[] PROGMEM        = { "SRV_ST_TIMEOUT" };
const char str_srv_st_fail[] PROGMEM           = { "SRV_ST_FAIL" };

// Vettore in program memory per i nomi degli stati
const char* const state_name[] PROGMEM = {
     str_srv_st_init,
     str_srv_st_idle,
     str_srv_st_sending_only,
     str_srv_st_sending,
     str_srv_st_waiting_answ,
     str_srv_st_done,
     str_srv_st_error,
     str_srv_st_fail
};

// Risposte accettabili da parte del server
const char str_srv_answ_OK[] PROGMEM           = { "OK" };
const char str_srv_answ_RELOAD[] PROGMEM       = { "RISP_RICARICA_REGOLARE" };
const char str_srv_answ_RESO_OK[] PROGMEM      = { "RISP_RESO_REGOLARE" };
const char str_srv_answ_RESO_DONE[] PROGMEM    = { "RISP_RESO_EFFETTUATO" };
const char str_srv_answ_RESO_KO[] PROGMEM      = { "RISP_RESO_NON_AUTORIZZATO" };
const char str_srv_answ_ANNULLATO[] PROGMEM    = { "RISP_PRELIEVO_ANNULLATO" };
//BADGE_NrComputer_badge
const char str_srv_answ_BDG_OK[] PROGMEM       = { "RISP_BADGE_ABILITATO" };
const char str_srv_answ_BDG_KO[] PROGMEM       = { "RISP_BADGE_NON_ABILIATO" };
const char str_srv_answ_BDG_OOT[] PROGMEM      = { "RISP_BADGE_FUORI_ORARIO" };
const char str_srv_answ_BDG_CEASED[] PROGMEM   = { "RISP_BADGE_CESSATO" };
//PIATTO_
const char str_srv_answ_PRODUCT_NENA[] PROGMEM = { "RISP_PRODOTTO_NON_ABILITATO" };
const char str_srv_answ_MAX_LIM[] PROGMEM      = { "RISP_LIMITE_MAX_RAGGIUNTO" };
const char str_srv_answ_PREL_OK[] PROGMEM      = { "RISP_PRELIEVO_REGOLARE" };
const char str_srv_answ_PREL_LATE[] PROGMEM    = { "RISP_PRELIEVO_IN_RITARDO" };


// numero di tentativi di connessione
#define SERV_MAX_RETRY 3

// prototipi funzione locali
static void print_open_status(void);
BOOL server_open(void);
void server_close(void);
void server_transmit(void);
BOOL server_invia_messaggio_senza_risposta(void);

static void server_print_answer_code(server_answer_code code);
static BOOL Server_check_answer(server_msg_id msg_id, char* server_answer);
static server_answer_code Server_decode_answer(server_msg_id msg_id, char* server_answer);

// Prototipi di funzioni esterne
extern BOOL verifica_congruita_config_ricevuto(void);
//=====================================================================//

/*****************************************************/
/* function Server_init()                            */
/* Inizializza la macchina a stati di gestione della */
/* logica del server.                                */
/*****************************************************/
void server_init(void)
  {    
     srv_dbg = TRUE;

     debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_server_init_start);

     fsm_init(&Server_FSM,          // Puntatore alla struttura della macchina a stati
              SRV_ST_INIT,          // Stato iniziale
              (void*)state_name,          // Array contenente i nomi degli stati
              MAX_SRV_STATE,       // Massimo valore dello stato
              "SRV ");           // nome della macchina a stati
     
     fsm_ena_print(&Server_FSM, srv_dbg, srv_dbg, srv_dbg);

     // imposta i parametri di comunicazione con il server
     server_set_connection_param();

     // inzializza le utility usate dalla gestione del server
     server_utility_init();

     // effettua la prima connessione
     if (server_LAN_connection())
       {
          server_work.LAN_connection_OK = TRUE;
          debug_message_timestamp_PGM(str_LAN_connect_OK);
          Ethernet_print_info();
       }
     else
       {
          server_work.LAN_connection_OK = FALSE;
          debug_message_timestamp_PGM(str_LAN_connect_KO);
       }
     last_server_event = SRV_EV_NONE;

     debug_message_timestamp_PGM(str_server_init_end);
  }

/*------------------------------------------------------------------*/
/* function server_reset_error()                                    */
/* Azzera una eventuale condizione di errore del gestore del server */
/*------------------------------------------------------------------*/
void server_reset_error(void)
  {
  }

/*****************************************************************/
/* function Server_request_send()                                */
/* Invia un evento al gestore del server.                        */
/* In ingresso riceve :                                          */
/* - prm_msg_id     = id tipo di messaggio (ACCESA,PCTIME,ecc)   */
/* - usr_parameter  = parametro opzionale                        */
/* In funzione dell'id del tipo di messaggio viene selezionato   */
/* il tipo di servizio da passare alla gestione del server ,     */
/* ovvero se messaggio tx_only oppure messaggio con risposta     */
/* In uscita restituisce TRUE se il servizioni e' stato accodato */
/* o FALSE se vi erano attivita pendenti                         */
/*****************************************************************/
BOOL server_request_send(server_msg_id prm_msg_id, char* usr_parameter)
  {
     BOOL retval;
     retval = FALSE;

     // nome funzione
     debug_message_timestamp_PGM(str_srv_request_send);

     // info ID e evento
     char buff[50];
     char loc_param[10];
     if (strlen(usr_parameter) == 0)
       strncpy(loc_param, "none", sizeof(loc_param) - 1);
     else
       strncpy(loc_param, usr_parameter, sizeof(loc_param) - 1);
     sprintf(buff, " [ id=%s  prm=%s ] ", serv_ID_to_str(prm_msg_id), loc_param);
     debug_message_timestamp(buff);

     // verifica che non vi siano attivita pendenti
     if (server_work.usr_msg_id == SRV_MSGID_NONE)
       {
          // memorizza ID del messaggio da inviare
          server_work.usr_msg_id = prm_msg_id;
          switch (server_work.usr_msg_id)
            {
               // Servizi che non prevedono risposta
               case SRV_MSGID_ACCESA:
               case SRV_MSGID_APERTA:
               case SRV_MSGID_CHIUSA:
               case SRV_MSGID_RELOAD:
               case SRV_MSGID_REFILL:
               case SRV_MSGID_RICARICATO:
                 server_work.usr_msg_type = SRV_EV_SEND_ONLY;
                 break;
               // Servizi per cui è necessaria una risposta dal server
               case SRV_MSGID_ANAG:
               case SRV_MSGID_RICHIESTA_CONFIG:
               case SRV_MSGID_PCTIME:
               case SRV_MSGID_CHECK_CFG:
               case SRV_MSGID_BADGE:
               case SRV_MSGID_AUTORIZZO:
               case SRV_MSGID_TRANSAZIONE:
               case SRV_MSGID_PIATTO:
               case SRV_MSGID_RESO:
               case SRV_MSGID_ANNULLATO:
               case SRV_MSGID_SEND_OFFLINE:
                 server_work.usr_msg_type = SRV_EV_SEND_AND_WAIT;
                 break;
               default:
                 server_work.usr_msg_type = SRV_EV_NONE;
            }
          if (server_work.usr_msg_type != SRV_EV_NONE)
            {
               strncpy(usr_message_payload, usr_parameter, SERVER_MAX_MESSAGE);

               debug_message_timestamp_PGM(str_srv_msg_in_carico);
               retval = TRUE;
            }
          else
            {
               debug_message_timestamp_PGM(str_srv_msg_non_accodato);
               retval = FALSE;
            }
       }
     else
       {
          debug_message_timestamp_PGM(str_srv_msg_non_accodato);
          retval = FALSE;
       }

     return retval;
  }

static char server_answer[80]; // dati ultima risposta dal server
/***************************************************************************/
/* function Server_hand()                                                  */
/* Funzione che gestisce la comunicazione con il  server.                  */
/* Viene chiamata dal main globale di applicazione in free-run             */
/* Scoda le richieste di servizi inserite tramite la "Server_request_send" */
// E' normalmente in una condizione di attesa nello stato "SRV_ST_IDLE"
// La "server_request_send" inserisce l'ID del servizio da eseguire
// in "last_server_event"
// Esistono due tipi di attivita:
// - con semplice invio (SRV_EV_SEND_ONLY)
// - con invio e attesa risposta (SRV_EV_SEND_AND_WAIT)
// La selezione del tipo di attivita viene effettuato dalla "server_request_send"
// in funzione del tipo di interrogazione richiesta.
// Questa funzione si appoggia ad alcune funzioni ed in particolare :
// - server_invia_messaggio_senza_risposta()
// - server_invia_messaggio_con_risposta()
// - server_read_answer()

// - Server_check_answer()
// - Server_decode_answer()

// Queste funzioni di supporto sono inserite nella "server_utility" e sono
// utlizzate internamente dalla 
// - server_invia_messaggio_senza_risposta
// - server_invia_messaggio_con_risposta
// - server_read_answer

// A sua volta si appggiano alcune utility e in particolare:
//
// - server_make_message()
//   e' costrituita da uno switch che crea il pacchetto dello specifico messaggio
//   costituito da una componente fissa determinata dallo specifico messaggio e da 
//   una parte opzionale passata come parametro alla "Server_request_send" 
//   es : BADGE_ richiede come parametro il numero del badge
//
// - server_open()
//   apre la connessione al server con i parametri preimpostati effettuando opportuni
//   retry in caso di problemi
//
// - server_transmit() 
//   apre la connessione e invia inserendo i preamboli richiesti 
//
// - server_close() chiude la connessione (subito per messaggi senza risposta o 
//   dopo la risposta negli altri casi)
//
// - server_start_read()
//   inizializza la procedura di ricezione effettuata da una FSM che analizza il flusso di 
//   di caratteri in arrivo dal server
// 
// - server_read_answer()
//   Funzione strutturata a FSM che legge carattere per carattere i dati in arrivo 
//   - riconosce l'inizio del payload con keyword #PAY_STR
//   - salva riga per riga i dati del payload fino a trovare la keyword #PAY_END
//   - nel caso di interrogazioni di parametri semplici (ovvero di una sola riga) la
//     risposta viene inserita nel parametro "answer" della chiamata
//   - nel caso do risposte con dati multilinea (config e anagrafica) la 
//     server_read_answer si avvale di due chiamate a funzione specifiche che
//     che memorizzano direttamente i dati ricevuti. 
//     Nel caso del config i dato in arrivo vengono memorizzati in una struttura di
//     tipo "config" di appoggio , per una successiva analisi e memorizzazione.
//     Nel caso della anagrafica attualmente salva direttamente riga per riga nel
//     file su SD anag.DAT
//

// Flusso attivita di una singola operazione di interrogazione del server
// 1) Dalla condizione di attesa viene chiamata o la 
// server_invia_messaggio_senza_risposta() oppure la server_invia_messaggio_con_risposta()
// Entrambe queste funzioni non sono bloccanti . 
//
// 2) Nel caso di messaggi senza risposta il gestore del server torna immediatamente in attesa.
//
// 3) Nel caso di messaggio con risposta viene chiamata la "server_read_answer" per leggere
// e analizzare la risposta del server. Questa funzione si conclude con la letture e
// restituzione dei dati e il timeout in caso di problemi di connessione.
// La funzione server_read_answer() e' bloccante a livello di risposta ma non a livello
// di applicazione (FSM+millis)
// I dati ricevuti come risposta vengono inseriti nel buffer "server_answer" passato come
// parametro ad eccezione di config e anagrafiche che per dimensione vengono salvati
// da apposite chiamate a funzioni specifiche di salvataggio.
//
// 4) La Server_check_answer() effettua una prima macro verifica del contenuto della
// risposta per evitare di analizzare i dati completamente incrongruenti.
// Attualmente verifica semplicemente che la stringa di risposta non sia  "#RISP_BADGE_SCONOSCIUTO" 
// - possibile miglioria : server potrebbe inserire nella testata dell risposta una stringa
//   che identifica il dato inviato  es :#PCTIME xxxxxxxxx
// 
// 5) La Server_decode_answer() decodifica e analizza i dati ricevuti , caso per caso
//    appoggiandosi per le specifiche azioni ad apposite API 
//    nb: le API specifiche di analisi delle singole risposte sono inserite nei
//        modulo che utilizza quel servzio (orologio per PCTIME,gestione config x config,ecc

/*****************************************************/
//Server_request_service
void server_request_hand(void)
  {
     switch (Server_FSM.state)
       {
          case SRV_ST_INIT:               
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_init)))
              {
              }
            fsm_set_state(&Server_FSM, SRV_ST_IDLE);
            break;

          // attesa arrivo richieste servizi
          case SRV_ST_IDLE:
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_idle)))
              {
              }
            if (server_work.usr_msg_id != SRV_MSGID_NONE)
              {
                 switch (server_work.usr_msg_type)
                   {
                      // invio senza risposta
                      case SRV_EV_SEND_ONLY:
                        fsm_set_state(&Server_FSM, SRV_ST_SENDING_ONLY);
                        break;
                      // invio con rispoasta
                      case SRV_EV_SEND_AND_WAIT:
                        fsm_set_state(&Server_FSM, SRV_ST_SENDING);
                        break;
                      default:
                        server_work.usr_msg_id = SRV_MSGID_NONE;
                        server_work.usr_msg_type = SRV_EV_NONE;
                    } 
              }
            break;

          // gestione servizi di invio messaggi senza risposta
          case SRV_ST_SENDING_ONLY:
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_sending_only)))
              {
                 ;
              }
            // Invia il messaggio al server
            // In uscita restituisce TRUE se l'invio e andato a buon fine 
            // (verifica se la server_open e' riuscita)
            // Effettua una open,una write e una close.
            if (server_invia_messaggio_senza_risposta())
              {
                 debug_message_timestamp_PGM(str_srv_send_OK);
              }
            else
              {
                 debug_message_timestamp_PGM(str_srv_send_KO);
              }
            // cancella rirchiesta pendente
            server_work.usr_msg_id = SRV_MSGID_NONE;
            server_work.usr_msg_type = SRV_EV_NONE;
            fsm_set_state(&Server_FSM, SRV_ST_IDLE);
            break;

          // gestione servizi di invio messaggi con risposta
          case SRV_ST_SENDING:
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_sending)))
              {
                 ;
              }
            // Invia il messaggio al server
            // In uscita restituisce TRUE se l'invio e andato a buon fine 
            // (verifica se la server_open e' riuscita)
            // Effettua una open,una write ma lascia la connessione aperta
            if (server_invia_messaggio_con_risposta())
              {
                 debug_message_timestamp_PGM(str_srv_send_OK);
              }
            else
              {
                 debug_message_timestamp_PGM(str_srv_send_KO);
              }
            fsm_set_state(&Server_FSM, SRV_ST_WAITING_ANSW);
            break;

          // stiamo attendendo la risposta dal server
          case SRV_ST_WAITING_ANSW:
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_waiting_answ)))
              { 
                    ;
              }
            else
              {
                 // attendi la risposta
                 uint8_t esito;
                 // verificare in "server_read_answer" il comportamento con stringhe mal formattate (da server)
                 // Simulare l'interruzione di comunicazione durante l'invio o la ricezione di un comando
                 esito = server_read_answer(server_answer, sizeof(server_answer));  // Da verificare cosa restituisce per implementare errori
                 switch (esito)
                   {
                      // se passa di qua ha ottenuto una risposta formalmente accettbile
                      // #PAYLOAD_STR ...... #PAYLOAD_END
                      case FSM_DONE:
                        //server_first = TRUE;                         
                        char buff[150];
                        sprintf(buff, "risposta server = %s", server_answer);
                        debug_message_timestamp(buff);

                        //debug_print_timestamp_ident(TRUE, DEBUG_IDENT_L1, "imposto data_ora :");

                        // esegue un controllo validita sul contenuto della risposta
                        // verifica che la risposta non sia "INVALID COMMAND"
                        if (Server_check_answer(server_work.usr_msg_id, server_answer))
                          {
                             sprintf(buff,"msg_id:%02X",server_work.usr_msg_id);
                             debug_message_timestamp(buff);
                             // seleziono e gestisco i vari casi della risposta
                             server_answer_code esito = Server_decode_answer(server_work.usr_msg_id, server_answer);

                             // visualizza i vari esiti
                             #ifdef DEBUG_VERSION
                             server_print_answer_code(esito);
                             #endif

                             switch (esito)
                               {
                                  case SRV_ANSW_OK:     // risposta corretta
                                    fsm_set_state(&Server_FSM, SRV_ST_DONE);
                                    break;
                                  case SRV_ANSW_DATA_ERR:   // errore nei dati in risposta
                                  case SRV_ANSW_UNKONW:     // errore di sistema
                                  case SRV_ANSW_INVALID_MSGID: // id non previsto
                                    fsm_set_state(&Server_FSM, SRV_ST_ERROR);
                                    break;
                                  default:
                                    ; // errore interno all'applicazione
                               }
                                   
                          }
                        else
                          {
                             fsm_set_state(&Server_FSM, SRV_ST_ERROR);
                          }                              
                        break;

                      // esce in caso di dimeout
                      case FSM_TIMEOUT:
                        //server_first = TRUE;
                        fsm_set_state(&Server_FSM, SRV_ST_TIMEOUT);
                        break;
                           
                   }
              }
            break;

          case SRV_ST_DONE:
            // Aggiungere contatore comandi eseguiti
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_done)))
              {
                 // cancella rirchiesta pendente
                 server_work.usr_msg_id = SRV_MSGID_NONE;
                 server_work.usr_msg_type = SRV_EV_NONE;
                 // attiva ritardo richiesta successiva      
                 fsm_set_timer(&Server_FSM, FSM_TIMER1, SRV_DONE_DELAY);
              }
            else
              {
                // tempo di attesa dopo comando OK
                if (fsm_check_end_time(&Server_FSM, FSM_TIMER1))
                  {
                     fsm_set_state(&Server_FSM, SRV_ST_IDLE);
                  }
              }
            break;

          case SRV_ST_ERROR:
            // Aggiungere contatore errori sui comandi
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_error)))
              {
                 // cancella rirchiesta pendente
                 server_work.usr_msg_id = SRV_MSGID_NONE;
                 server_work.usr_msg_type = SRV_EV_NONE;
                 // attiva ritardo richiesta successiva      
                 fsm_set_timer(&Server_FSM, FSM_TIMER1, SRV_ERROR_DELAY);
              }
            else
              {
                 // tempo di attesa dopo errore
                 if (fsm_check_end_time(&Server_FSM, FSM_TIMER1))
                   {
                      fsm_set_state(&Server_FSM, SRV_ST_IDLE);
                   }
              }
            break;

          case SRV_ST_TIMEOUT:
            // Aggiungere contatore errori sui comandi
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_timeout)))
              {
                 // cancella rirchiesta pendente
                 server_work.usr_msg_id = SRV_MSGID_NONE;
                 server_work.usr_msg_type = SRV_EV_NONE;
                 // attiva ritardo richiesta successiva      
                 fsm_set_timer(&Server_FSM, FSM_TIMER1, SRV_ERROR_DELAY);
              }
            // tempo di attesa dopo errore
            if (fsm_check_end_time(&Server_FSM, FSM_TIMER1))
              {
                 fsm_set_state(&Server_FSM, SRV_ST_IDLE);
              }
            break;

          case SRV_ST_FAIL:
            if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_fail)))
              {
                    // tipicamente, condizione non recuperabile
              }
            break;

          default:
               ;
     }
  }

//===================================================================================================================//

static void server_print_answer_code(server_answer_code code)
  {
     BOOL local_print_ena;
     local_print_ena = TRUE;
     switch (code)
       {
          case SRV_ANSW_OK:     // risposta corretta
            debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_answ_OK_dbg));
            break;
          case SRV_ANSW_DATA_ERR:   // errore nei dati in risposta
            debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_answ_data_err));
            break;
          case SRV_ANSW_UNKONW:     // errore di sistema
            debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_sys_err));
            break;
          case SRV_ANSW_INVALID_MSGID: // id non previsto
            debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_invalid_msgid));
            break;
          default:
            debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_impossible));
       }
  }

// verifica che la risposta del server non sia di un servizio conosciuto 
static BOOL Server_check_answer(server_msg_id msg_id, char* server_answer)
  {    
     if (strncmp(server_answer, "INVALID_COMMAND", 15) == 0)
          return FALSE;
     else
          return TRUE;
  }

/*--------------------------------------------------------*/
/* gestisce i vari casi delle risposte a seguito di una   */
/* interrogazione al server                               */
/*--------------------------------------------------------*/
static server_answer_code Server_decode_answer(server_msg_id msg_id, char* server_answer)
  {
     server_answer_code retval;

     retval = SRV_ANSW_UNKONW;
     switch (msg_id)
       {
          case SRV_MSGID_ACCESA:
            break; 
          
          case SRV_MSGID_APERTA:
            break; 
          
          case SRV_MSGID_CHIUSA:
            break; 
          
          case SRV_MSGID_ANAG:
            break;
          
          case SRV_MSGID_RICHIESTA_CONFIG:
            if (verifica_congruita_config_ricevuto())
              retval = SRV_ANSW_OK;
            else
              retval = SRV_ANSW_DATA_ERR;
            break; 
          
          case SRV_MSGID_PCTIME:
            if (set_data_ora_da_server(server_answer))
              retval = SRV_ANSW_OK;
            else
              retval = SRV_ANSW_DATA_ERR;
            break; 
          
          case SRV_MSGID_CHECK_CFG:
            break; 
          
          case SRV_MSGID_RELOAD:
            break; 
          
          case SRV_MSGID_REFILL:
            break; 
          
          case SRV_MSGID_RICARICATO:
            break; 
          
          case SRV_MSGID_BADGE:
            break; 
          
          case SRV_MSGID_AUTORIZZO:
            break; 
          
          case SRV_MSGID_TRANSAZIONE:
            break; 
          
          case SRV_MSGID_PIATTO:
            break; 
          
          case SRV_MSGID_RESO:
            break; 
          
          case SRV_MSGID_ANNULLATO:
            break; 
          
          case SRV_MSGID_SEND_OFFLINE:
            break; 
          
          default:
            retval = SRV_ANSW_INVALID_MSGID;
     }
     
     return retval;
  }

/*------------------------------------------------------------------------------------*/
/* restituisce lo stato della FSM di controllo HL del gestore degli eventi del server */
/*------------------------------------------------------------------------------------*/
FSM_RETVAL server_request_state(void)
  {
     FSM_RETVAL retval;

     switch (Server_FSM.state)
       {
          case SRV_ST_IDLE:
            retval = FSM_READY;
            break;
          case SRV_ST_INIT:
            retval = FSM_NOT_INIT;
            break;
          case SRV_ST_SENDING_ONLY:
          case SRV_ST_SENDING:
          case SRV_ST_WAITING_ANSW:
            retval = FSM_EXEC;
            break;
          case SRV_ST_DONE:
            retval = FSM_DONE;
            break;
          case SRV_ST_ERROR:
            retval = FSM_ERROR;
            break;
          case SRV_ST_FAIL:
            retval = FSM_FAIL;
            break;
          default:
            retval = FSM_INVALID_STATE;
       }
     return retval;
  }

#undef MODULE_APPL_SERVER

