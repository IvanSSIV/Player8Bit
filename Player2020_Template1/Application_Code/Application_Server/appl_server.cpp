/****************************************************************************/
/* File: appl_server.cpp                                                    */
/* Data creazione: 07/04/2020  												*/
/* Creato da: Ivan De Stefani  												*/
/* Funzione: Gestione ad alto livello della logica del server.    			*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/
#define MODULE_APPL_SERVER
#include "player_header.h"


extern IPAddress IP;
extern IPAddress GW;
extern IPAddress DNS;
extern IPAddress SM;

// Definizione della variabile per lo stato della FSM di gestione del server
FSM_WORK Server_FSM;

static char* state_name[] =
{
	"SRV_INIT",
	"SRV_IDLE",
	"SRV_SENDING_ONLY",
	"SRV_SENDING",
	"SRV_WAITING_ANSW",
	"SRV_ST_DONE",
	"SRV_ST_ERROR",
	"SRV_ST_FAIL"
};

IPAddress SERVER(0, 0, 0, 0);							// IP del server
uint32_t port;											// Porta del server

// Stringhe costanti per debug e invio al server
const char str_server_init[] PROGMEM = { "Inizializzo logica server" };
const char str_LAN_connect_OK[] PROGMEM = { "Connessione con server stabilita ..." };
const char str_LAN_connect_KO[] PROGMEM = { "Impossibile connettersi al server ..." };
const char str_srv_st_init[] PROGMEM = { "SRV_ST_INIT" };
const char str_srv_st_idle[] PROGMEM = { "SRV_ST_IDLE" };
const char str_srv_st_sending_only[] PROGMEM = { "SRV_ST_SENDING_ONLY" };
const char str_srv_st_sending[] PROGMEM = { "SRV_ST_SENDING" };
const char str_srv_st_waiting_answ[] PROGMEM = { "SRV_ST_WAITING_ANSW" };
const char str_srv_st_done[] PROGMEM = { "SRV_ST_DONE" };
const char str_srv_st_error[] PROGMEM = { "SRV_ST_ERROR" };
const char str_srv_st_fail[] PROGMEM = { "SRV_ST_FAIL" };
const char str_srv_send_OK[] PROGMEM = { "Invio messaggio riuscito" };
const char str_srv_send_KO[] PROGMEM = { "Invio messaggio fallito" };
const char str_srv_send_event[] PROGMEM = { "server send event :" };

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

//=====================================================================//

/*****************************************************/
/* function Server_init()                            */
/* Inizializza la macchina a stati di gestione della */
/* logica del server.                                */
/*****************************************************/
void Server_init(void)
  {
	
	debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_server_init));
	fsm_init(&Server_FSM,		// Puntatore alla struttura della macchina a stati
			 SRV_ST_INIT,		// Stato iniziale
			 state_name,		// Array contenente i nomi degli stati
			 MAX_SRV_STATE,  	// Massimo valore dello stato
		     "SRV ");           // nome della macchina a stati
	// Abilito la stampa degli stati
	char buff[50];
	sprintf(buff, "srv_dbg = %0d", srv_dbg);
	debug_print_timestamp(DBG_ALWAYS_ON, buff);
	fsm_ena_print(&Server_FSM, srv_dbg, srv_dbg, srv_dbg);

	// imposta i parametri di comunicazione con il server
	server_set_connection_param();

	// effettua la prima connessione
	if (server_LAN_connection())
	  {
		 LAN_connection_OK = TRUE;
		 debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L1, AVR_PGM_to_str(str_LAN_connect_OK));
		 Ethernet_print_info();
	  }
	else
	  {
		 LAN_connection_OK = FALSE;
		 debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L1, AVR_PGM_to_str(str_LAN_connect_KO));
	  }
	last_server_event = SRV_EV_NONE;
  }

static char server_answer[80]; // dati ultima risposta dal server
/*****************************************************/
/* function Server_hand()                            */
/* Funzione che gestisce la comunicazione con il     */
/* server.                                           */
/*****************************************************/
//Server_request_service
void Server_request_hand(void)
{
	switch (Server_FSM.state)
	{
		case SRV_ST_INIT:
			
			if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_init)))
			{
				//LCD_send_event(LCD_EVENT_UPDATE_LINE2, AVR_PGM_to_str(str_srv_st_init));
			}
			fsm_set_state(&Server_FSM, SRV_ST_IDLE);
			break;

	    // attesa arrivo eventi
		case SRV_ST_IDLE:
			if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_idle)))
			  {
	
			  }
			switch (last_server_event)
			  {
				 // invio senza risposta
			     case SRV_EV_SEND_ONLY:
				   fsm_set_state(&Server_FSM, SRV_ST_SENDING_ONLY);
				   break;
				 // invio con rispsoat
				 case SRV_EV_SEND_AND_WAIT:
				   fsm_set_state(&Server_FSM, SRV_ST_SENDING);
				   break;

			  } 
			break;

		case SRV_ST_SENDING_ONLY:

			if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_sending_only)))
				;
			if (server_invia_messaggio_senza_risposta())
			  {
				 debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_send_OK));
			  }
			else
			  {
				debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_send_KO));
			  }
			last_server_event = SRV_EV_NONE;
			fsm_set_state(&Server_FSM, SRV_ST_IDLE);
			break;

		case SRV_ST_SENDING:
			if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_sending)))
				;
			if (server_invia_messaggio_con_risposta())
			{
				debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_send_OK));
			}
			else
			{
				debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_send_KO));
			}
			last_server_event = SRV_EV_NONE;
			fsm_set_state(&Server_FSM, SRV_ST_WAITING_ANSW);
			break;

		case SRV_ST_WAITING_ANSW:

			if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_waiting_answ)))
			  { 
				;
			  }
			else
			  {
				 // attendi la risposta
				 uint8_t esito;
				 esito = server_read_answer(server_answer, sizeof(server_answer));  // Da verificare cosa restituisce per implementare errori
				 switch (esito)
				   {
				      case FUNC_RET_DONE:
					    //server_first = TRUE;
					    char buff[150];
					    sprintf(buff, "risposta server = %s", server_answer);
					    debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, buff);
					    //debug_print_timestamp_ident(TRUE, DEBUG_IDENT_L1, "imposto data_ora :");

						// esegue un controllo validita sul contenuto della risposta
						if (Server_check_answer(usr_msg_id, server_answer))
						  {
							// seleziono e gestisco i vari casi della risposta
							server_answer_code esito = Server_decode_answer(usr_msg_id, server_answer);

							server_print_answer_code(esito);
							fsm_set_state(&Server_FSM, SRV_ST_DONE);
						  }
						else
					      {
							fsm_set_state(&Server_FSM, SRV_ST_ERROR);
						  }
					    
						
				    	break;
				      case FUNC_RET_ERROR:
					    //server_first = TRUE;
						fsm_set_state(&Server_FSM, SRV_ST_FAIL);
					    break;
					  
				   }
			  }
			break;

		case SRV_ST_DONE:
			// Aggiungere contatore comandi eseguiti
			if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_done)))
			  {
				 fsm_set_timer(&Server_FSM, FSM_TIMER1);
			  }
			if (fsm_timer_hand(&Server_FSM, FSM_TIMER1, SRV_DONE_DELAY))
			  {
				 fsm_set_state(&Server_FSM, SRV_ST_IDLE);
			  }
			break;

		case SRV_ST_ERROR:
			// Aggiungere contatore errori sui comandi
			if (fsm_first_scan(&Server_FSM, AVR_PGM_to_str(str_srv_st_error)))
			{
				fsm_set_timer(&Server_FSM, FSM_TIMER1);
			}
			if (fsm_timer_hand(&Server_FSM, FSM_TIMER1, SRV_ERROR_DELAY))
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


static void server_print_answer_code(server_answer_code code)
{
	BOOL local_print_ena;
	local_print_ena = TRUE;
	switch (code)
	{
	case SRV_ANSW_OK:     // risposta corretta
		debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, (char*)"risposta server OK");
		break;
	case SRV_ANSW_DATA_ERR:   // errore nei dati in risposta
		debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, (char*)"dati risposta errati");
		break;
	case SRV_ANSW_UNKONW:     // errore di sistema
		debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, (char*)"errore di sistema ");
		break;
	case SRV_ANSW_INVALID_MSGID: // id non previsto
		debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, (char*)"id messaggio non supportato");
	default:
		debug_print_timestamp_ident(local_print_ena, DEBUG_IDENT_L1, (char*)"impossibile che succeda ....");
	}
}

static BOOL Server_check_answer(server_msg_id msg_id, char* server_answer)
{
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
	//
	
	return retval;
  }
/************************************************************/
/* function Server_send_event()                             */
/* Invia un evento al gestore del server.                   */
/* - event = tipo di azione da compiere                     */
/* - prm_msg_id = id tipo di messaggio (ACCESA,PCTIME,ecc)  */
/* - messaggio  = parametro opzionale                       */
/************************************************************/
BOOL Server_request_send(uint8_t event, SRV_MSG_ID prm_msg_id,char *usr_payload)
{
    // nome funzione
	debug_print_timestamp_ident(srv_dbg, DEBUG_IDENT_L1, AVR_PGM_to_str(str_srv_send_event));
	
	// info ID e evento
	char buff[50],event_buff[6];
	serv_event_to_str(event, event_buff, sizeof(event_buff));

	sprintf(buff, " [ event=%s id=%s ] ", event_buff, serv_ID_to_str(prm_msg_id));
	debug_print_ena(srv_dbg, buff);
	
	// visualizza payload
	if (strlen(usr_payload) != 0)
	{
		debug_print_ena(srv_dbg, " ");
		debug_print_ena(srv_dbg, usr_payload);
	}

	// visualizza ed elabora risultato della send event
	if (last_server_event == SRV_EV_NONE)
	{
		last_server_event = event;

		usr_msg_id = prm_msg_id;
		strncpy(usr_message_payload, usr_payload, SERVER_MAX_MESSAGE);
		debug_print_ena(srv_dbg, (char*)" *** messaggio preso in carico ***");
		return TRUE;
	}
	else
	{
		debug_print_ena(srv_dbg, (char*)" *** messaggio non accodato ***");
		return FALSE;
	}
}

/*------------------------------------------------------------------------------------*/
/* restituisce lo stato della FSM di controllo HL del gestore degli eventi del server */
/*------------------------------------------------------------------------------------*/

event_hand_retval Server_get_event_hand_state(void)
  {
	event_hand_retval temp_retval;
	switch (Server_FSM.state)
	{
		case SRV_ST_IDLE:
			temp_retval = EVENT_HAND_READY;
			break;
		case SRV_ST_INIT:
			temp_retval = EVENT_HAND_STARTING;
			break;
		case SRV_ST_SENDING_ONLY:
		case SRV_ST_SENDING:
		case SRV_ST_WAITING_ANSW:
			temp_retval = EVENT_HAND_EXEC;
			break;
		case SRV_ST_DONE:
			temp_retval = EVENT_HAND_DONE;
			break;
		case SRV_ST_ERROR:
			temp_retval = EVENT_HAND_ERROR;
			break;
		case SRV_ST_FAIL:
		default:
			temp_retval = EVENT_HAND_FAIL;
	}
	return temp_retval;
  }

#undef MODULE_APPL_SERVER

