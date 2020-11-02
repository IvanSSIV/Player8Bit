#define MODULE_SRV_UTILS

// include comune librerie LANZI
#include "../Lanzi_library.h"
#include "IPAddress.h"
#include "EthernetClient.h"

#define SERVER_RX_INIT     0
#define SERVER_RX_HEADER   1
#define SERVER_RX_PAYLOAD  2
#define SERVER_RX_VERIFY   3
#define SERVER_RX_END      4
#define SERVER_RX_ERROR    5
#define SERVER_RX_TIMEOUT  6

#define SERVER_TIMEOUT 6000L

// Variabili pubbliche
extern IPAddress IP;
extern IPAddress GW;
extern IPAddress DNS;
extern IPAddress SM;
extern IPAddress SERVER;
extern uint32_t port;

// Stringhe costanti per stampa debug
const char str_ok[] PROGMEM               = { "Ok" };
const char str_fail[] PROGMEM             = { "Fail" };
const char str_func_send_noansw[] PROGMEM = { "Function server_invia_messaggio_senza_risposta()..." };
const char str_srv_timeout[] PROGMEM      = { "Server timeout" };
const char str_rx_line[] PROGMEM          = { "Rx_line :" };
const char str_rx_pay_start[] PROGMEM     = { "Inizio rx_payload ..." };
const char str_rx_data[] PROGMEM          = { "Rx_data : " };
const char str_header[] PROGMEM           = { "Header : " };
const char str_rx_pay_end[] PROGMEM       = { "Fine rx_payload ..." };
const char str_accesa[] PROGMEM           = { "ACCESA_" };
const char str_aperta[] PROGMEM           = { "APERTA_" };
const char str_chiusa[] PROGMEM           = { "CHIUSA_" };
const char str_rich_anag[] PROGMEM        = { "RICHIESTA_ANAG_" };
const char str_rich_config[] PROGMEM      = { "RICHIESTA_CONFIG_" };
const char str_rich_pctime[] PROGMEM      = { "RICHIESTA_PCtime_" };
const char str_check_config[] PROGMEM     = { "CHECK_CONFIG_" };
const char str_ricaricato[] PROGMEM       = { "RICARICATO_" };
const char str_reload[] PROGMEM           = { "RELOAD_" };
const char str_refill[] PROGMEM           = { "REFILL_" };
const char str_badge[] PROGMEM            = { "BADGE_" };
const char str_autorizzo[] PROGMEM        = { "AUTORIZZO_" };
const char str_transazione[] PROGMEM      = { "TRANSAZIONE_" };
const char str_piatto[] PROGMEM           = { "PIATTO_" };
const char str_annullato[] PROGMEM        = { "ANNULLATO_" };
const char str_reso[] PROGMEM             = { "RESO_" };
const char str_srv_status[] PROGMEM       = { "Server status: " };
const char str_func_srv_close[] PROGMEM   = { "function server_close()..." };
const char str_eth_info_start[] PROGMEM   = { "Informazioni connessione Ethernet:" };
const char str_eth_info_end[] PROGMEM     = { "Fine informazioni connessione Ethernet." };
const char str_impostazioni[] PROGMEM     = { "Impostazioni server IP:%03u-%03u-%03u-%03u  porta:%05u"};
const char str_esito_conn[] PROGMEM       = { "Test connessione LAN  tentativo:%02u-%02u  t_ms:%05lu  esito:%s "};
// Variabili locali

static EthernetClient client;    // Costruttore della classe EthernetClient per la connessione al server



static BOOL server_connection;          // stato connnessione attuale
static BOOL server_last_connection;     // esito dell'ultima connessione
static char tx_message[SERVER_TX_SIZE]; // buffer dati da inviare (header + payload)
static uint32_t server_open_cnt;        // conteggio numero aperture
static uint32_t server_open_err;        // conteggio errori apertura
static uint32_t server_transmit_cnt;    // conteggio pacchetti inviati
static uint32_t server_receive_cnt;     // conteggio pacchetti ricevuti correttamente
static uint32_t server_timeout_cnt;     // conteggio timeout
static uint8_t  server_rx_stat;         // stato ricezione
static uint32_t start_tx;               // timestamp all'inizio della trasmissione
static uint32_t end_tx;                 // timestamp alla fine della trasmissione
static uint32_t start_rx;               // timestamp all'arrivo del primo carattere
static uint32_t timeout_rx;


// Prototipi di funzione
static void server_make_message(void);
static BOOL server_find_init_payload(char* buff);
static void server_visualizza_dati_ricevuti(BOOL ena, uint8_t ident, char* buff, const char* intestazione);
static BOOL server_find_end_payload(char* buff);
static void server_salva_riga_ricevuta(char* buff);
static void memorizza_risposta(char* answer, uint16_t answ_size);
static void server_clear_rx_buff(void);
static BOOL server_get_payload_start(char c);

// prototipi di funzione esterne
// funzioni presenti in "config_lib.cpp"
extern void config_save_start(char* line);
extern void config_save_line(char* line);
extern void config_save_end(char* line);

void server_utility_init(void)
  {
     memset(&server_work, 0, sizeof(server_work));
  }
  
/*------------------------------------------------*/
/* function server_set_connection_param()         */
/* Preleva dal config i parametri di connessione  */
/*------------------------------------------------*/
//#define TEST_SERVER 
void server_set_connection_param(void)
  {
     MY_IP_ADDR serv_IP;
     char buff[80];

     //serv_IP = config_get_serv_IP();
     #ifdef TEST_SERVER
     serv_IP.oct1 = 193;
     serv_IP.oct2 = 204;
     serv_IP.oct3 = 114;
     serv_IP.oct4 = 232;

     //port = config_get_serv_port();
     port = 123; // 1443
     #else
     serv_IP.oct1 = 212;
     serv_IP.oct2 = 112;
     serv_IP.oct3 = 86;
     serv_IP.oct4 = 16;

     //port = config_get_serv_port();
     port = 1443; // 1443
     #endif
     SERVER[0] = serv_IP.oct1;
     SERVER[1] = serv_IP.oct2;
     SERVER[2] = serv_IP.oct3;
     SERVER[3] = serv_IP.oct4;
     sprintf(buff,
             AVR_PGM_to_str(str_impostazioni),
             SERVER[0], SERVER[1], SERVER[2], SERVER[3], port);
     debug_message_timestamp(buff);
  }


void arduino_extern_print(char* message)
{
        debug_print_timestamp(TRUE, message);
}

/*---------------------------------------------*/
/* function server_LAN_connection()            */
/* Effettua il test di connessione al server   */
/* Restituisce TRUE se connessione avvenuta    */
/*---------------------------------------------*/
#define SERVER_OPEN_MAX_RETRY 6
BOOL server_LAN_connection(void)
  {
     BOOL retval;
     char buff[120],esito[10];
     uint8_t retry;
     uint32_t time_open;
        
     retval = FALSE;
     debug_message_ident_push();
     //client.stop();
     for (retry = 1; retry <= SERVER_OPEN_MAX_RETRY; retry++)
       {
          char buff[80];
          sprintf(buff,
                  "server:%03u-%03u-%03u-%03u port:%04u",
                  SERVER[0],
                  SERVER[1],
                  SERVER[2],
                  SERVER[3],
                  port);
          debug_message_timestamp(buff);
          server_work.open_ms_start = millis();
          open_connect_retval = client.connect(SERVER, port);
          server_work.open_ms_end = millis();

          time_open = server_work.open_ms_end - server_work.open_ms_start;

          if (open_connect_retval)
            {
               strncpy_AVR_PGM(esito,str_ok,sizeof(esito));
            }
          else
            {
               strncpy_AVR_PGM(esito,str_fail,sizeof(esito));
            }

          sprintf(buff,
                  AVR_PGM_to_str(str_esito_conn),
                  retry, 
                  SERVER_OPEN_MAX_RETRY, 
                  time_open,
                  esito);
          debug_message_timestamp(buff);
  
          if (open_connect_retval)
            {
               client.stop();
               retval = TRUE;
               break;
            }
          else
            {
               delay(1000);
            }
        }
     debug_message_ident_pop();
     return retval;
  }

BOOL server_first_connection(void)
  {

  }

/*------------------------------------------------------*/
/* function server_invia_messaggio_senza_risposta()     */
/* Invia un messaggio al server che non richiede        */
/* una risposta per il proseguimento del funzionamento. */
/*------------------------------------------------------*/
BOOL server_invia_messaggio_senza_risposta(void)
  {
     BOOL connect;

     #ifdef DEBUG_VERSION
     debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_func_send_noansw));
     #endif

     // apri la connessione
     connect = server_open();
     if (connect)
       {
          // compone il messge
          server_make_message();
          // invia il messaggio
          server_transmit();
          // chiudi la connessione
          server_close();
       }

     return connect;
  }

/*------------------------------------------------------------------*/
/* function server_invia_messaggio_con_risposta()                   */
/* Effettua tutte le operazioni per inviare un messaggio al server  */
/* e attenderme la risposta , ovvero :                              */
/* - preparazione del messaggio da inviare                          */
/* - apertura della connessione                                     */
/* - invio del messaggio                                            */
/* - inizializzazione della ricezione                               */
/*------------------------------------------------------------------*/
BOOL server_invia_messaggio_con_risposta(void)
  {
     BOOL connect;

     // apri la connessione
     connect = server_open();

     if (connect)
       {
          // compone il messge
          server_make_message();

          // invia il messaggio
          server_transmit();

          server_transmit_cnt++;

          // avvia la ricezione
          server_start_read();

       }

     return connect;
  }

static BOOL server_read_rx_line(void);

/*--------------------------------------------------------------------------*/
/* attende una risposta dal server in modo non sospensivo                   */
/* Al temine delle operazioni "answer" contiene la stringa con la rispota   */
/* Come valore di ritorno restituisce lo stato di esecuzione "FUNC_RET_xx"  */
/*--------------------------------------------------------------------------*/
uint8_t server_read_answer(char *answer,uint16_t answ_size)
  {
     uint8_t retval;
     
     retval = FSM_EXEC;
     
     // verifica fine tempo 
     if ((RTOS_get_tick_1ms()- start_rx) >= SERVER_TIMEOUT)
       {
          #ifdef DEBUG_VERSION
          debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_timeout));
          debug_print_timestamp_ident(srv_dbg,DEBUG_IDENT_L1, AVR_PGM_to_str(str_rx_line));
          debug_print_ena(srv_dbg,server_work.rx_line);
          #endif
          server_rx_stat = SERVER_RX_TIMEOUT;
       }
       
     switch (server_rx_stat)
       {
          // condizione iniziale
          case SERVER_RX_INIT:
            memset(server_work.rx_line,0,sizeof(server_work.rx_line));
            memset(server_work.last_rx_data,0,sizeof(server_work.last_rx_data));
                        server_work.rx_index = 0;
            server_work.init_payload = FALSE;
            server_rx_stat = SERVER_RX_HEADER;
            break;
            
          // lettura header della risposta  
          case SERVER_RX_HEADER:
            // leggi una riga dal server
            if (server_read_rx_line())
              {
                 // aspetta inizio payload
                 // nb: la "server_find_init_payload" detecta la condizione di inizio payload
                 //     all'interno delle informazioni ricevute dal server
                 if (server_work.init_payload)
                   {
                      #ifdef DEBUG_VERSION
                      debug_print_timestamp_ident(srv_dbg,DEBUG_IDENT_L1, AVR_PGM_to_str(str_rx_pay_start));
                      #endif  
                      // visualizza dati ricevuti
                      // nb: nel caso di "read_config" e "read_anag" non fa nulla
                      #ifdef DEBUG_VERSION
                      server_visualizza_dati_ricevuti(srv_dbg,DEBUG_IDENT_L1, server_work.rx_line, str_rx_data); // era str_rx_pay_end ??
                      #endif

                      // se siamo in lettura config inizializza ricezione
                      if (server_work.usr_msg_id == SRV_MSGID_RICHIESTA_CONFIG)
                        {
                           config_save_start(server_work.rx_line);
                        }
                      // se siamo in lettura anagrafica inizializza ricezione
                      if (server_work.usr_msg_id == SRV_MSGID_ANAG)
                        {
                           anag_save_start(server_work.rx_line);
                        }
                      
                      // passa a leggere il payload
                      server_rx_stat = SERVER_RX_PAYLOAD;
                   }
                 else
                   {
                      // visualizza dati ricevuti
                      // nb: passa di qui per scaricare i dati inviati prima del #PAY_STR
                      #ifdef DEBUG_VERSION
                      server_visualizza_dati_ricevuti(server_work.ena_header_print,DEBUG_IDENT_L1, server_work.rx_line, str_header);
                      #endif
                   }
                   
                 // ripulisci il buffer per la riga successiva
                 server_clear_rx_buff();
              }
            break;
            
          // lettura del payload  
          case SERVER_RX_PAYLOAD:
            // leggi una riga dal server
            if (server_read_rx_line())
              {                 
                 // visualizza dati ricevuti
                 #ifdef DEBUG_VERSION
                 server_visualizza_dati_ricevuti(srv_dbg,DEBUG_IDENT_L1, server_work.rx_line, str_rx_data);
                 #endif
                 // vedi se fine payload
                 if (server_find_end_payload(server_work.rx_line))
                   {
                      // segnala fine payload
                      #ifdef DEBUG_VERSION
                      debug_print_timestamp_ident(srv_dbg,DEBUG_IDENT_L1, AVR_PGM_to_str(str_rx_pay_end));
                      #endif
                      server_rx_stat = SERVER_RX_END;               
                   }
                 else
                   {
                      // memorizza i dati ricevuti
                      server_salva_riga_ricevuta(server_work.rx_line);
                   }  
                 
                 // ripulisci il buffer per la riga successiva
                 server_clear_rx_buff();
              }
            break;
            
          // ricezione risposta completata
          case SERVER_RX_END:
            server_receive_cnt++;
            // se siamo in lettura config chiudi la ricezione
            if (server_work.usr_msg_id == SRV_MSGID_RICHIESTA_CONFIG)
              {
                 config_save_end(server_work.rx_line);
              }
            // se siamo in lettura anagrafica chiudi la ricezione
            if (server_work.usr_msg_id == SRV_MSGID_ANAG)
              {
                 anag_save_end(server_work.rx_line);
              }
            // prepara la risposta 
            if (answer != NULL)
              {
                 // copia nel nel buffer della funzione chiama
                 memorizza_risposta(answer,answ_size); 
              }
            server_close();
            retval = FSM_DONE;
            break;
            
          // avvenuto timeout
          case SERVER_RX_TIMEOUT:
            // ?? da verificare 
            // server_close();
            server_timeout_cnt++;
            retval = FSM_TIMEOUT;
            break;
       }
       
     return retval;
  }  

void server_start_read(void)
  {
     server_clear_rx_buff();
     server_rx_stat = SERVER_RX_INIT;
     start_rx = millis();
     server_work.init_payload = FALSE;
     //timeout_rx = start_rx + SERVER_TIMEOUT;
  }

static void server_clear_rx_buff(void)
  {
     memset(server_work.rx_line, 0, sizeof(server_work.rx_line));
     server_work.rx_index = 0;
  }

/*--------------------------------------------------------------*/
/* function server_read_rx_line()                               */
/* Legge una riga dal client                                    */
/* restituisce TRUE all'arrivo di una stringa completa          */
/* Esegue un loop di lettura dei caratteri in arrivo dal server */
/* Esce alla fine di una riga completa (CR/LF) oppure se il     */
/* buffer del server e' vuoto . Se esce per buffer vuoto        */
/* restituisce FALSE                                            */
/* Analizza il flusso di caratteri in arrivo alla ricerca della */
/* stringa "#PAY_STR" che identifica l'inizio del payloada e    */
/* setta opportunamento il flag "init_payload"                  */
/*--------------------------------------------------------------*/
static BOOL server_read_rx_line(void)
  {
     short c;
     BOOL retval, exit_flg;

     exit_flg = FALSE;
     retval = FALSE;
     while (client.available() && !exit_flg)
       {
          c = client.read();
                        //debug_write_char(c);
          switch (c)
            {
               // vedi se LF
               case 0x0A:
                 server_work.paydec_state = 0;
                 break;
               // se CR ho finito
               case 0x0D:
                 exit_flg = TRUE;
                 retval = TRUE;
                 break;
               // tutti gli altri caratteri salvali nel/ buffer
               default:
                 if (server_get_payload_start(c))
                   {
                      server_work.init_payload = TRUE;
                   }
                 if (server_work.rx_index < (SERVER_RX_LINE_SIZE - 2))
                   {
                      server_work.rx_line[server_work.rx_index] = c;
                      server_work.rx_index++;
                   }
                 else
                   {
                      server_work.rx_line[server_work.rx_index] = c;
                      server_work.rx_index++;
                      exit_flg = TRUE;
                      retval = TRUE;
                   }
            }

        }

     return retval;
  }

/*--------------------------------------------------------*/
/* function server_get_payload_start()                    */
/* Cerca la presenza della stringa "#PAY_STR" all'interno */
/* del flusso di caratteri in arrivo . La ricerca avviene */
/* con una macchina a stati e non dipende dalla posizione */
/* del messaggio all'interno della stringa .              */
/* Restituisce TRUE se viene trovata la stringa           */
/*--------------------------------------------------------*/
static BOOL server_get_payload_start(char c)
  {
     BOOL retval;
     
     retval = FALSE;
     switch (server_work.paydec_state)
       {
          case 0:
            if (c == '#')
              server_work.paydec_state = 1;
            break;
          case 1:
            if (c == 'P')
              server_work.paydec_state = 2;
            else
              server_work.paydec_state = 0;
            break;
          case 2:
            if (c == 'A')
              server_work.paydec_state = 3;
            else
              server_work.paydec_state = 0;
            break;
          case 3:
            if (c == 'Y')
              server_work.paydec_state = 4;
            else
              server_work.paydec_state = 0;
            break;
          case 4:
            if (c == '_')
              server_work.paydec_state = 5;
            else
              server_work.paydec_state = 0;
            break;
          case 5:
            if (c == 'S')
              server_work.paydec_state = 6;
            else
              server_work.paydec_state = 0;
            break;
          case 6:
            if (c == 'T')
              server_work.paydec_state = 7;
            else
              server_work.paydec_state = 0;
            break;
          case 7:
            if (c == 'R')
              {
                 retval = TRUE;
              }
            else
              server_work.paydec_state = 0;
            break;
       }
     return retval;
  }

/*-----------------------------------------------*/
/* function server_find_init_payload()           */
/* Verifica l'inizio del payload della risposta  */
/*-----------------------------------------------*/
static BOOL server_find_init_payload(char* buff)
  {
     // da chiarire 
     if (server_work.usr_msg_id == SRV_MSGID_SEND_OFFLINE)
       {
          if (strncmp(buff, "<br", 3) == 0)
            {
               return TRUE;
            }
            
          if (strstr(buff, "#PAY_STR"))
            return TRUE;

          return FALSE;
       }
     else
       {
          if (strstr(buff, "#PAY_STR"))
            return TRUE;
          else
            return FALSE;
       }
   }

/*-----------------------------------------------*/
/* function server_find_init_payload()           */
/* Verifica la fine del payload della risposta   */
/*-----------------------------------------------*/
static BOOL server_find_end_payload(char* buff)
  {
     if (strstr(buff, "#PAY_END"))
       return TRUE;
     else
       return FALSE;
  }

/*-----------------------------------------------------------------------*/
/* function server_salva_riga_ricevuta()                                 */
/* Salva una riga di dati ricevuta dal server                            */
/* Nei casi di risposte su una singola riga copia l'ultima riga ricevuta */
/* "server_data.rx_line" nel buffer per fornire la risposta al chiamante */
/* Nei casi di config e anagrafica chiama apposite funzioni di utility   */
/*-----------------------------------------------------------------------*/
static void server_salva_riga_ricevuta(char* buff)
   {
      switch (server_work.usr_msg_id)
         {
            case SRV_MSGID_PCTIME:
            case SRV_MSGID_CHECK_CFG:
            case SRV_MSGID_BADGE:
            case SRV_MSGID_AUTORIZZO:
            case SRV_MSGID_SEND_OFFLINE:
            case SRV_MSGID_PIATTO:
              strcpy(server_work.last_rx_data, server_work.rx_line);
              break;
            case SRV_MSGID_RICHIESTA_CONFIG:
              config_save_line(server_work.rx_line);
              break;
            case SRV_MSGID_ANAG:
              anag_save_line(server_work.rx_line);
              break;
         }

   }

static void memorizza_risposta(char* answer, uint16_t answ_size)
  {
     switch (server_work.usr_msg_id)
       {
          case SRV_MSGID_PCTIME:
          case SRV_MSGID_CHECK_CFG:
          case SRV_MSGID_BADGE:
          case SRV_MSGID_AUTORIZZO:
          case SRV_MSGID_SEND_OFFLINE:
          case SRV_MSGID_PIATTO:
            strncpy(answer, server_work.last_rx_data, answ_size);
            break;

          case SRV_MSGID_RICHIESTA_CONFIG:
            break;
          case SRV_MSGID_ANAG:
            break;
       }
   }
 
/*---------------------------------------------------*/
/* function server_visualizza_dati_ricevuti()        */
/* Visualizza una riga di dati ricevuto dal server   */
/*---------------------------------------------------*/
static void server_visualizza_dati_ricevuti(BOOL ena, uint8_t ident, char* buff, const char* intestazione)
  {
     if (strlen(buff) > 0)
       {
          debug_print_timestamp_ident(ena, ident, AVR_PGM_to_str(intestazione));
          debug_print_ena(ena, buff);
       }
  }
 
// Modificare per prelevare i dati dalla struttura CONFIG globale
static void get_config_numero_PC(char* name, uint8_t size)
  {
     strncpy(name, "55555", size - 1);
     strncat(name, "_", size - 1);
     name[size - 1] = 0;
  }
  
static void get_firmware_versione(char* fwvers, uint8_t size)
  {
     strncpy(fwvers, "0.90.09", size - 1);
     strncat(fwvers, "_", size - 1);
     fwvers[size - 1] = 0;
  }

static void serv_strncat(char* message)
  {
     strncat(tx_message_payload, message, sizeof(tx_message_payload) - 1);
  }

static void serv_msg_start(const char* message)
  {
     strncpy(tx_message_payload, AVR_PGM_to_str(message), sizeof(tx_message_payload) - 1);
  }

/*----------------------------------------------*/
/* function server_make_message()               */
/* Prepara i vari messaggi da inviare al server */
/*----------------------------------------------*/
static void server_make_message(void)
  {
     char numero_PC[10];
     char versione_FW[10];
     char datetime[10];

     get_config_numero_PC(numero_PC,sizeof(numero_PC)-1);
     get_firmware_versione(versione_FW, sizeof(versione_FW) - 1);
     get_datetime_ASCII_Lanzi(datetime, sizeof(datetime));

     switch (server_work.usr_msg_id)
       {
          case SRV_MSGID_ACCESA:
            serv_msg_start(str_accesa);
            serv_strncat(numero_PC);
            serv_strncat(versione_FW);
            serv_strncat(datetime);
            break;
            
          case SRV_MSGID_APERTA:
            serv_msg_start(str_aperta);
            serv_strncat(numero_PC);
            serv_strncat(datetime);
            break;

          case SRV_MSGID_CHIUSA:
            serv_msg_start(str_chiusa);
            serv_strncat(numero_PC);
            serv_strncat(datetime);
            break;
          
          case SRV_MSGID_ANAG:
            serv_msg_start(str_rich_anag);
            serv_strncat(numero_PC);
            serv_strncat(datetime);
            break;
                
          case SRV_MSGID_RICHIESTA_CONFIG:
            serv_msg_start(str_rich_config);
            serv_strncat(numero_PC);
            serv_strncat(datetime);
            break;
          
          case SRV_MSGID_PCTIME:
            serv_msg_start(str_rich_pctime);
            serv_strncat(numero_PC);
            serv_strncat(datetime);
            break;
          
          case SRV_MSGID_CHECK_CFG:
            serv_msg_start(str_check_config);
            serv_strncat(numero_PC);
            serv_strncat(datetime);
            break;
            
          case SRV_MSGID_RELOAD:
            serv_msg_start(str_reload);
            serv_strncat(numero_PC);
            break;
          
          case SRV_MSGID_REFILL:
            serv_msg_start(str_refill);
            serv_strncat(numero_PC);
            break;
          
          case SRV_MSGID_RICARICATO:
            serv_msg_start(str_ricaricato);
            serv_strncat(numero_PC);
            serv_strncat(datetime);
            break;
          
          case SRV_MSGID_BADGE:
            serv_msg_start(str_badge);
            serv_strncat(usr_message_payload);
            serv_strncat(numero_PC);
            break;
            
          case SRV_MSGID_AUTORIZZO:
            serv_msg_start(str_autorizzo);
            serv_strncat(usr_message_payload);
            serv_strncat(numero_PC);
            break;
            
          case SRV_MSGID_TRANSAZIONE:
            serv_msg_start(str_transazione);
            serv_strncat(usr_message_payload);
            serv_strncat(numero_PC);
            break;
            
          case SRV_MSGID_PIATTO:
            serv_msg_start(str_piatto);
            serv_strncat(usr_message_payload);
            serv_strncat(numero_PC);
            break;
            
          case SRV_MSGID_RESO:
            serv_msg_start(str_reso);
            serv_strncat(usr_message_payload);
            serv_strncat(numero_PC);
            break;
          
          case SRV_MSGID_ANNULLATO:
            serv_msg_start(str_annullato);
            serv_strncat(usr_message_payload);
            serv_strncat(numero_PC);
            break;
          
          default:
            break;
       }
     debug_print_timestamp_title(srv_dbg, DEBUG_IDENT_L1, "Messaggio inviato: ", tx_message_payload);
  }

/*--------------------------------------------------*/
/* function print_open_status()                     */
/* Stampa il risultato dei tentativi di connessione */
/* al server.                                       */
/*--------------------------------------------------*/
char  buff34[80];
static void print_open_status(void)
  {
     char esito[10];
     uint32_t time_open;

     if (open_connect_retval)
       strcpy(esito, (char*)"Ok ..  ");
     else
       strcpy(esito, (char*)"Fail ..");
     time_open = server_work.open_ms_end - server_work.open_ms_start;

     #ifdef DEBUG_VERSION
     sprintf(buff34, "%s (retval=%03d time:%04lu)", esito, open_connect_retval, time_open);
     debug_print_timestamp_title(srv_dbg, DEBUG_IDENT_L1, "Server status: ", buff34);
     #endif
  }


/*--------------------------------------------------------*/
/* function server_open()                                 */
/* Prova a connetersi al server . In uscita restituisce   */
/* TRUE = se connessione andata a buon fine               */
/* FALSE = se connessione fallita                         */
/*--------------------------------------------------------*/
BOOL server_open(void)
  {
     uint8_t retry;
     char buff[80];

     for (retry = 1; retry <= SERVER_OPEN_MAX_RETRY; retry++)
       {
          #ifdef DEBUG_VERSION
          sprintf(buff, "function server_open() tentativo:%02d-%02d : ", retry, SERVER_OPEN_MAX_RETRY);
          debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, buff);
          #endif

          server_work.open_ms_start = millis();
          open_connect_retval = client.connect(SERVER, port);
          server_open_cnt++;
          server_work.open_ms_end = millis();

          // visualizza esito operazione
          print_open_status();
          if (open_connect_retval)
            {
               // connessione avvenuta
               server_connection = TRUE;
               break;
            }
          else
            {
               // mancata connessione
               delay(100);
               server_open_err++;
            }
       }

     server_last_connection = server_connection;

     return server_connection;
  }

/*---------------------------------------------*/
/* function server_close()                     */
/* Chiude la connessione al server             */
/*---------------------------------------------*/
void server_close(void)
  {
     server_connection = FALSE;
     //client.flush();
     uint32_t start_ms = RTOS_get_tick_1ms();
     client.stop();
     uint32_t end_ms = RTOS_get_tick_1ms();

     #ifdef DEBUG_VERSION
     debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_func_srv_close));    
     #endif
  }

/*---------------------------------------------*/
/* function server_transmit()                  */
/* Compone il messaggio per la trasmissione    */
/* al server.                                  */
/*---------------------------------------------*/
void server_transmit(void)
  {
        char buff[100];

        // prepara il messaggio completo
        strcpy(tx_message, "GET /player32.php?msg=");
        strcat(tx_message, tx_message_payload);
        strcat(tx_message, " HTTP/1.0");

        start_tx = millis();

#ifdef DEBUG_VERSION
        debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, (char*)"tx_msg : ");
        debug_print_ena(srv_dbg, tx_message);
#endif

        // invia il messaggio
        client.println(tx_message);

        // invia la chiusura
        client.print("Host: ");
        client.print(SERVER);
        client.print(":");
        client.println(port);
        client.println();

#ifdef DEBUG_VERSION
        sprintf(buff, "host   : (ip_addr=%03d:%03d:%03d:%03d  port=%05u)",
                SERVER[0],
                SERVER[1],
                SERVER[2],
                SERVER[3],
                port);
        debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, buff);
#endif

        end_tx = millis();
}
/*************************************************/
/* function Ethernet_print_info()                */
/* Stampa le informazioni sulla connessione      */
/*************************************************/
void Ethernet_print_info(void)
  {
        char buff[50];
        debug_print_timestamp(srv_dbg, AVR_PGM_to_str(str_eth_info_start));
        sprintf(buff, "IP Macchina: %03d.%03d.%03d.%03d", IP[0], IP[1], IP[2], IP[3]);
        debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "IP Server : %03d.%03d.%03d.%03d", SERVER[0], SERVER[1], SERVER[2], SERVER[3]);
        debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "Porta    : %05d", port);
        debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "Gateway  : %03d.%03d.%03d.%03d", GW[0], GW[1], GW[2], GW[3]);
        debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "DNS: %03d.%03d.%03d.%03d", DNS[0], DNS[1], DNS[2], DNS[3]);
        debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "Subnet mask: %03d.%03d.%03d.%03d", SM[0], SM[1], SM[2], SM[3]);
        debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
        debug_print_timestamp(srv_dbg, AVR_PGM_to_str(str_eth_info_end));
  }


char * serv_ID_to_str(SRV_MSG_ID ID)
  {
     uint8_t locsize;
     static char buff[20];

     locsize = sizeof(buff) - 1;
     switch (ID)
       {
          case SRV_MSGID_ACCESA:
            strncpy(buff, AVR_PGM_to_str(str_accesa), locsize);
            break;
          case SRV_MSGID_APERTA:
            strncpy(buff, AVR_PGM_to_str(str_aperta), locsize);
            break;
          case SRV_MSGID_CHIUSA:
            strncpy(buff, AVR_PGM_to_str(str_chiusa), locsize);
            break;
          case SRV_MSGID_ANAG:
            strncpy(buff, AVR_PGM_to_str(str_rich_anag), locsize);
            break;
          case SRV_MSGID_RICHIESTA_CONFIG:
            strncpy(buff, AVR_PGM_to_str(str_rich_config), locsize);
            break;
          case SRV_MSGID_PCTIME:
            strncpy(buff, AVR_PGM_to_str(str_rich_pctime), locsize);
            break;
          case SRV_MSGID_CHECK_CFG:
            strncpy(buff, AVR_PGM_to_str(str_check_config), locsize);
            break;
          case SRV_MSGID_RELOAD:
            strncpy(buff, AVR_PGM_to_str(str_reload), locsize);
            break;
          case SRV_MSGID_REFILL:
            strncpy(buff, AVR_PGM_to_str(str_refill), locsize);
            break;
          case SRV_MSGID_RICARICATO:
            strncpy(buff, AVR_PGM_to_str(str_ricaricato), locsize);
            break;
          case SRV_MSGID_BADGE:
            strncpy(buff, AVR_PGM_to_str(str_badge), locsize);
            break;
          case SRV_MSGID_AUTORIZZO:
            strncpy(buff, AVR_PGM_to_str(str_autorizzo), locsize);
            break;
          case SRV_MSGID_TRANSAZIONE:
            strncpy(buff, AVR_PGM_to_str(str_transazione), locsize);
            break;
          case SRV_MSGID_PIATTO:
            strncpy(buff, AVR_PGM_to_str(str_piatto), locsize);
            break;
          case SRV_MSGID_RESO:
            strncpy(buff, AVR_PGM_to_str(str_reso), locsize);
            break;
          case SRV_MSGID_ANNULLATO:
            strncpy(buff, AVR_PGM_to_str(str_annullato), locsize);
            break;
          default:
            break;
        }

     return buff;
  }

void serv_event_to_str(uint8_t event, char* msg, uint8_t size)
  {
     switch (event)
       {
          case SRV_EV_SEND_AND_WAIT:
            strncpy(msg, "SNW", size - 1);
            break;
          case SRV_EV_SEND_ONLY:
            strncpy(msg, "SND", size - 1);
            break;
          default:
            strncpy(msg, "???", size - 1);
        }
  }  

#undef MODULE_SRV_UTILS
