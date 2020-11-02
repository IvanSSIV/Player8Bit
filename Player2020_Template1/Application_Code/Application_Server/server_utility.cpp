#define MODULE_SRV_UTILS

// include standard / Arduino
#include "stdint.h"
#include "string.h"
#include "Arduino.h"
#include "SPI.h"
#include <Ethernet.h>
#include <utility/w5100.h>

// include di applicazione
#include "debug_opzion.h"
#include "project_def.h"
#include "My_Include/mydef.h"
#include "My_Include/my_time.h"
#include "My_Include/global_define.h"
#include "My_Debug/Debug_Serial/debug_serial.h"
#include "My_Debug/Debug_Utility/debug_utility.h"
#include "My_Debug/Serial_Command/serial_command.h"
#include "Hardware/CPU_core/timing.h"

#include <Application_Code/Application_Utility/appl_utility.h>
#include "My_Library/Utilities/AVR_API.h"
#include "My_Library/Connessione/Ethernet_template.h"
#include "My_Library/RTC/RTC_template.h"
#include "appl_server.h"
#include "server_utility.h"

#define SERVER_RX_INIT     0
#define SERVER_RX_HEADER   1
#define SERVER_RX_PAYLOAD  2
#define SERVER_RX_VERIFY   3
#define SERVER_RX_END      4
#define SERVER_RX_ERROR    5

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
const char str_chech_config[] PROGMEM     = { "CHECK_CONFIG_" };
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

// Variabili locali

EthernetClient client;									// Costruttore della classe EthernetClient per la connessione al server

static BOOL server_connection;                  // stato connnessione attuale
static BOOL server_last_connection;             // esito dell'ultima connessione
static char tx_message[SERVER_TX_SIZE];         // buffer dati da inviare (header + payload)

static char rx_buffer[SERVER_RX_SIZE];          // buffer dati grezzi da server
#define SERVER_RX_LINE_SIZE 128
static char rx_line[SERVER_RX_LINE_SIZE];       // dati riga corrente 
static char last_rx_data[SERVER_RX_LINE_SIZE];  // dati ultima riga dati ricevuta

static uint32_t server_open_cnt;				// conteggio numero aperture
static uint32_t server_open_err;				// conteggio errori apertura
static uint32_t server_transmit_cnt;			// conteggio pacchetti inviati
static uint32_t server_receive_cnt;				// conteggio pacchetti ricevuti correttamente
static uint32_t server_timeout_cnt;				// conteggio timeout
static uint8_t  server_rx_stat;                 // stato ricezione
//static uint8_t  messaggio_corrente;              // tipo di messaggio corrente
static uint16_t rx_index;
static uint32_t start_tx;		                // timestamp all'inizio della trasmissione
static uint32_t end_tx;					        // timestamp alla fine della trasmissione
static uint32_t start_rx;						// timestamp all'arrivo del primo carattere
static uint32_t timeout_rx;
static BOOL init_payload;

// Prototipi di funzione
static void server_make_message(void);
static BOOL server_find_init_payload(char* buff);
static void server_visualizza_dati_ricevuti(BOOL ena, uint8_t ident, char* buff, const char* intestazione);
static BOOL server_find_end_payload(char* buff);
static void server_salva_riga_ricevuta(char* buff);
static void memorizza_risposta(char* answer, uint16_t answ_size);


/*------------------------------------------------*/
/* function server_set_connection_param()         */
/* Preleva dal config i parametri di connessione  */
/*------------------------------------------------*/
void server_set_connection_param(void)
{
	MY_IP_ADDR serv_IP;
	char buff[80];

	//serv_IP = config_get_serv_IP();
	serv_IP.oct1 = 192;
	serv_IP.oct2 = 168;
	serv_IP.oct3 = 137;
	serv_IP.oct4 = 1;
	SERVER[0] = serv_IP.oct1;
	SERVER[1] = serv_IP.oct2;
	SERVER[2] = serv_IP.oct3;
	SERVER[3] = serv_IP.oct4;

	//port = config_get_serv_port();
	port = 8080;

	sprintf(buff, "Impostazioni server IP:%03u-%03u-%03u-%03u  porta:%05u",
		SERVER[0], SERVER[1], SERVER[2], SERVER[3], port);
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
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
	char buff[120];
	uint8_t retry;
	uint32_t time_open;
	

	retval = FALSE;
	for (retry = 1; retry <= SERVER_OPEN_MAX_RETRY; retry++)
	{
		open_ms_start = millis();
		open_connect_retval = client.connect(SERVER, port);
		open_ms_end = millis();

		time_open = open_ms_end - open_ms_start;
		sprintf(buff, "Test connessione LAN  tentativo:%02u-%02u  t_ms:%05lu  esito: ",
			retry, SERVER_OPEN_MAX_RETRY, time_open);
		debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);

		if (open_connect_retval)
		{
			debug_print_ena(srv_dbg, AVR_PGM_to_str(str_ok));
			client.stop();
			retval = TRUE;
			break;
		}
		else
		{
			debug_print_ena(srv_dbg, AVR_PGM_to_str(str_fail));
			delay(100);
		}
	}
	return retval;
}

/*------------------------------------------------------*/
/* function server_invia_messaggio_senza_risposta()     */
/* Invia un messaggio al server che non richiede        */
/* una risposta per il proseguimento del funzionamento. */
/*------------------------------------------------------*/
BOOL server_invia_messaggio_senza_risposta(void)
{
	BOOL connect;

	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_func_send_noansw));

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

/*---------------------------------------------------------*/
/* attende una risposta dal server in modo non sospensivo  */
/* Al temine delle operazioni "answer" contiene la stringa con la rispota   */
/* Come valore di ritorno restituisce lo stato di esecuzione "FUNC_RET_xx"  */
uint8_t server_read_answer(char *answer,uint16_t answ_size)
  {
     uint8_t retval;
     
     retval = FUNC_RET_EXEC;
     
	 // verifica fine tempo 
     if ((RTOS_get_tick_1ms()- start_rx) >= SERVER_TIMEOUT)
       {
		  debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_timeout));
          debug_print_timestamp_ident(srv_dbg,DEBUG_IDENT_L1, AVR_PGM_to_str(str_rx_line));
          debug_print_ena(srv_dbg,rx_line);
          server_rx_stat = SERVER_RX_ERROR;
       }
       
     switch (server_rx_stat)
       {
          // condizione iniziale
          case SERVER_RX_INIT:
            memset(rx_line,0,sizeof(rx_line));
            memset(last_rx_data,0,sizeof(last_rx_data));
            rx_index = 0;
            init_payload = FALSE;
            server_rx_stat = SERVER_RX_HEADER;
            break;
            
          // lettura header della risposta  
          case SERVER_RX_HEADER:
            // leggi una riga dal server
            if (server_read_rx_line())
              {
                 // aspetta inizio payload
                 if (server_find_init_payload(rx_line))
                   {
                      // segnala inizio payload
                      init_payload = TRUE;
                      debug_print_timestamp_ident(srv_dbg,DEBUG_IDENT_L1, AVR_PGM_to_str(str_rx_pay_start));
                      // visualizza dati ricevuti
                      // nb: nel caso di "read_config" e "read_anag" non fa nulla
                      server_visualizza_dati_ricevuti(srv_dbg,DEBUG_IDENT_L1,rx_line, str_rx_pay_end);
                      
                      // se siamo in lettura config inizializza ricezione
                      //if (messaggio_corrente == MSG_SERVER_READ_CONFIG)
                      //  config_save_start(rx_line);
                      // se siamo in lettura anagrafica inizializza ricezione
                      //if (messaggio_corrente == MSG_SERVER_READ_ANAG)
                      //  anagrafica_save_start(rx_line);
                      
                      // passa a leggere il payload
                      server_rx_stat = SERVER_RX_PAYLOAD;
                   }
                 else
                   {
                      // visualizza dati ricevuti
					  // nb: passa di qui per scaricare i dati inviati prima del #PAY_STR
                      server_visualizza_dati_ricevuti(srv_dbg,DEBUG_IDENT_L1,rx_line, str_header);
                   }
                   
                 // ripulisci il buffer per la riga successiva
                 memset(rx_line,0,sizeof(rx_line));
                 rx_index = 0;
              }
            break;
            
          // lettura del payload  
          case SERVER_RX_PAYLOAD:
            // leggi una riga dal server
            if (server_read_rx_line())
              {                 
                 // visualizza dati ricevuti
                 server_visualizza_dati_ricevuti(srv_dbg,DEBUG_IDENT_L1,rx_line, str_rx_data);
                 // vedi se fine payload
                 if (server_find_end_payload(rx_line))
                   {
                       // segnala fine payload
                      debug_print_timestamp_ident(srv_dbg,DEBUG_IDENT_L1, AVR_PGM_to_str(str_rx_pay_end));
                      server_rx_stat = SERVER_RX_END;               
                   }
                 else
                   {
                      // memorizza i dati ricevuti
                      server_salva_riga_ricevuta(rx_line);
                   }  
                 
                 // ripulisci il buffer per la riga successiva
                 memset(rx_line,0,sizeof(rx_line));
                 rx_index = 0;
              }
            break;
            
          // ricezione risposta completata
          case SERVER_RX_END:
            server_receive_cnt++;
            // se siamo in lettura config chiudi la ricezione
            //if (messaggio_corrente == SERVER_READ_CONFIG)
            //  {
            //     config_save_end(rx_line);
            //  }
            // prepara la risposta 
            if (answer != NULL)
              {
				 // copia nel nel buffer della funzione chiama
                 memorizza_risposta(answer,answ_size); 
              }
             
            retval = FUNC_RET_DONE;
            break;
            
          // avvenuto timeout
          case SERVER_RX_ERROR:
            server_timeout_cnt++;
            retval = FUNC_RET_ERROR;
            break;
       }
       
     return retval;
  }  

void server_start_read(void)
{
	memset(rx_buffer, 0, sizeof(rx_buffer));
	server_rx_stat = SERVER_RX_INIT;
	start_rx = millis();
	//timeout_rx = start_rx + SERVER_TIMEOUT;
}

/*-----------------------------------------*/
/* function server_read_rx_line()          */
/* Legge una riga dal client               */
/* restituisce TRUE all'arrivo di un CR    */
/*-----------------------------------------*/
 static BOOL server_read_rx_line(void)
	{
		short c;
		BOOL retval, exit_flg;

		exit_flg = FALSE;
		retval = FALSE;
		while (client.available() && !exit_flg)
		{
			c = client.read();
			switch (c)
			{
              // vedi se LF
			  case 0x0A:
				break;
              // se CR ho finito
			  case 0x0D:
				exit_flg = TRUE;
				retval = TRUE;
				break;
              // tutti gli altri caratteri salvali nel buffer
			  default:
				if (rx_index < SERVER_RX_LINE_SIZE - 1)
				{
					rx_line[rx_index] = c;
					rx_index++;
				}
			}
		}

		return retval;
	}

 /*-----------------------------------------------*/
/* function server_find_init_payload()           */
/* Verifica l'inizio del payload della risposta  */
/*-----------------------------------------------*/
 static BOOL server_find_init_payload(char* buff)
 {
	 if (usr_msg_id == SRV_MSGID_SEND_OFFLINE)
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

/*---------------------------------------------------*/
/* function server_salva_riga_ricevuta()             */
/* Salva una riga di dati ricevuta dal server        */
/*---------------------------------------------------*/
 static void server_salva_riga_ricevuta(char* buff)
 {
	 switch (usr_msg_id)
	 {
	 case SRV_MSGID_PCTIME:
	 case SRV_MSGID_CHECK_CFG:
	 case SRV_MSGID_BADGE:
	 case SRV_MSGID_AUTORIZZO:
	 case SRV_MSGID_SEND_OFFLINE:
	 case SRV_MSGID_PIATTO:
		 strcpy(last_rx_data, rx_line);
		 break;
	 case SRV_MSGID_RICHIESTA_CONFIG:
		 //config_save_line(rx_line);
		 break;
	 case SRV_MSGID_ANAG:
		 //anagrafica_save_line(rx_line);
		 break;
	 }

 }

 static void memorizza_risposta(char* answer, uint16_t answ_size)
 {
	 switch (usr_msg_id)
	 {
	 case SRV_MSGID_PCTIME:
	 case SRV_MSGID_CHECK_CFG:
	 case SRV_MSGID_BADGE:
	 case SRV_MSGID_AUTORIZZO:
	 case SRV_MSGID_SEND_OFFLINE:
	 case SRV_MSGID_PIATTO:
		 strncpy(answer, last_rx_data, answ_size);
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
static void get_config_numero_PC(char* name, uint8_t size)
  {
	 strncpy(name, "20221", size - 1);
	 strncat(name, "_", size - 1);
	 name[size - 1] = 0;
}
static void get_firmware_versione(char* fwvers, uint8_t size)
{
	strncpy(fwvers, "0.90.09", size - 1);
	strncat(fwvers, "_", size - 1);
	fwvers[size - 1] = 0;
}
/*static void get_date_time_Lanzi(char* datetime, uint8_t size)
{
	strncpy(datetime, "608135780", size-1);
	datetime[size - 1] = 0;
}*/

static void serv_strncat(char* message)
  {
	strncat(tx_message_payload, message, sizeof(tx_message_payload) - 1);
  }

static void serv_msg_start(const char* message)
  {
	//strncpy(tx_message_payload, message, sizeof(tx_message_payload) - 1);
	strncpy(tx_message_payload, AVR_PGM_to_str(message), sizeof(tx_message_payload) - 1);
}

static void server_make_message(void)
  {
	char numero_PC[10];
	char versione_FW[10];
	char datetime[10];

	get_config_numero_PC(numero_PC,sizeof(numero_PC)-1);
	get_firmware_versione(versione_FW, sizeof(versione_FW) - 1);
	get_datetime_ASCII_Lanzi(datetime, sizeof(datetime));
	switch (usr_msg_id)
	{
		case SRV_MSGID_ACCESA:
			serv_msg_start(str_accesa);
			serv_strncat(numero_PC);
			serv_strncat(versione_FW);
			serv_strncat(datetime);
			break;
		case SRV_MSGID_APERTA:
			//strncpy_AVR_PGM(dbgbuff, str_aperta, sizeof(dbgbuff) - 1);
			serv_msg_start(str_aperta);
			serv_strncat(numero_PC);
			serv_strncat(datetime);
			break;
		case SRV_MSGID_CHIUSA:
			//strncpy_AVR_PGM(dbgbuff, str_chiusa, sizeof(dbgbuff) - 1);
			serv_msg_start(str_chiusa);
			serv_strncat(numero_PC);
			serv_strncat(datetime);
			break;
		case SRV_MSGID_ANAG:
			//strncpy_AVR_PGM(dbgbuff, str_rich_anag, sizeof(dbgbuff) - 1);
			serv_msg_start(str_rich_anag);
			serv_strncat(numero_PC);
			serv_strncat(datetime);
			break;
		case SRV_MSGID_RICHIESTA_CONFIG:
			//strncpy_AVR_PGM(dbgbuff, str_rich_config, sizeof(dbgbuff) - 1);
			serv_msg_start(str_rich_config);
			serv_strncat(numero_PC);
			serv_strncat(datetime);
			break;
		case SRV_MSGID_PCTIME:
			//strncpy_P(dbgbuff, str_rich_pctime, sizeof(dbgbuff) - 1);
			serv_msg_start(str_rich_pctime);
			serv_strncat(numero_PC);
			serv_strncat(datetime);
			break;
		case SRV_MSGID_CHECK_CFG:
			//strncpy_AVR_PGM(dbgbuff, str_chech_config, sizeof(dbgbuff) - 1);
			serv_msg_start(str_chech_config);
			serv_strncat(numero_PC);
			serv_strncat(datetime);
			break;
		case SRV_MSGID_RELOAD:
			//strncpy_AVR_PGM(dbgbuff, str_reload, sizeof(dbgbuff) - 1);
			serv_msg_start(str_reload);
			serv_strncat(numero_PC);
			break;
		case SRV_MSGID_REFILL:
			//strncpy_AVR_PGM(dbgbuff, str_refill, sizeof(dbgbuff) - 1);
			serv_msg_start(str_refill);
			serv_strncat(numero_PC);
			break;
		case SRV_MSGID_RICARICATO:
			//strncpy_AVR_PGM(dbgbuff, str_ricaricato, sizeof(dbgbuff) - 1);
			serv_msg_start(str_ricaricato);
			serv_strncat(numero_PC);
			serv_strncat(datetime);
			break;
		case SRV_MSGID_BADGE:
			//strncpy_AVR_PGM(dbgbuff, str_badge, sizeof(dbgbuff) - 1);
			serv_msg_start(str_badge);
			serv_strncat(numero_PC);
			break;
		case SRV_MSGID_AUTORIZZO:
			//strncpy_P(dbgbuff, str_autorizzo, sizeof(dbgbuff) - 1);
			serv_msg_start(str_autorizzo);
			serv_strncat(numero_PC);
			break;
		case SRV_MSGID_TRANSAZIONE:
			//strncpy_AVR_PGM(dbgbuff, str_transazione, sizeof(dbgbuff) - 1);
			serv_msg_start(str_transazione);
			serv_strncat(numero_PC);
			break;
		case SRV_MSGID_PIATTO:
			//strncpy_AVR_PGM(dbgbuff, str_piatto, sizeof(dbgbuff) - 1);
			serv_msg_start(str_piatto);
			serv_strncat(numero_PC);
			break;
		case SRV_MSGID_RESO:
			//strncpy_AVR_PGM(dbgbuff, str_reso, sizeof(dbgbuff) - 1);
			serv_msg_start(str_reso);
			serv_strncat(numero_PC);
			break;
		case SRV_MSGID_ANNULLATO:
			//strncpy_AVR_PGM(dbgbuff, str_annullato, sizeof(dbgbuff) - 1);
			serv_msg_start(str_annullato);
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
/* al server.										*/
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
	time_open = open_ms_end - open_ms_start;
	sprintf(buff34, "%s (retval=%03d time:%04lu)", esito, open_connect_retval, time_open);
	debug_print_timestamp_title(srv_dbg, DEBUG_IDENT_L1, "Server status: ", buff34);
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
		sprintf(buff, "function server_open() tentativo:%02d-%02d : ", retry, SERVER_OPEN_MAX_RETRY);
		debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, buff);

		open_ms_start = millis();
		open_connect_retval = client.connect(SERVER, port);
		server_open_cnt++;
		open_ms_end = millis();

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
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_func_srv_close));    
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

	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, (char*)"tx_msg : ");
	debug_print_ena(srv_dbg, tx_message);

	// invia il messaggio
	client.println(tx_message);

	// invia la chiusura
	client.print("Host: ");
	client.print(SERVER);
	client.print(":");
	client.println(port);
	client.println();

	sprintf(buff, "host   : (ip_addr=%03d:%03d:%03d:%03d  port=%05u)",
		SERVER[0],
		SERVER[1],
		SERVER[2],
		SERVER[3],
		port);
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, buff);
	end_tx = millis();
}


/*************************************************/
/* function Ethernet_print_info()                */
/* Stampa le informazioni sulla connessione      */
/*************************************************/

void Ethernet_print_info(void)
{
	char buff[50];
	debug_print_timestamp(srv_dbg, (char*)"Informazioni connessione Ethernet:");
	sprintf(buff, "IP Macchina: %03d.%03d.%03d.%03d", IP[0], IP[1], IP[2], IP[3]);
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "IP Server: %03d.%03d.%03d.%03d", SERVER[0], SERVER[1], SERVER[2], SERVER[3]);
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "Porta: %05d", port);
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "Gateway: %03d.%03d.%03d.%03d", GW[0], GW[1], GW[2], GW[3]);
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "DNS: %03d.%03d.%03d.%03d", DNS[0], DNS[1], DNS[2], DNS[3]);
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "Subnet mask: %03d.%03d.%03d.%03d", SM[0], SM[1], SM[2], SM[3]);
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L2, buff);
	debug_print_timestamp(srv_dbg, (char*)"Fine informazioni connessione Ethernet.");
}


char * serv_ID_to_str(SRV_MSG_ID ID)
{
	uint8_t locsize;
    static char buff[5];

	locsize = strlen(buff) - 1;
	switch (ID)
	{
	case SRV_MSGID_ACCESA:
		strncpy(buff, AVR_PGM_to_str(str_accesa), locsize);
		break;
	case SRV_MSGID_APERTA:
		break;
	case SRV_MSGID_CHIUSA:
		break;
	case SRV_MSGID_ANAG:
		break;
	case SRV_MSGID_RICHIESTA_CONFIG:
		break;
	case SRV_MSGID_PCTIME:
		strncpy(buff, AVR_PGM_to_str(str_rich_pctime), locsize);
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
