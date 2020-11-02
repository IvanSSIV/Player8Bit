#ifdef MODULE_APPL_SERVER
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

// Define per gli stati della gestione logica della comunicazione con il server

#define MAX_SRV_STATE			7
#define SRV_ST_INIT				0  // condizione iniziale
#define SRV_ST_IDLE				1  // attesa comandi
#define SRV_ST_SENDING_ONLY		2  // invio senza risposta
#define SRV_ST_SENDING          3  // invio prima della risposta
#define SRV_ST_WAITING_ANSW		4  // attesa risposta
#define SRV_ST_DONE				5  // eseguito il comando
#define SRV_ST_ERROR			6  // errore recuperabile
#define SRV_ST_FAIL				7  // errore irrecuperabile, stato "incricched" o "pianted"

#define SRV_DONE_DELAY		500L   // millisecondi per i quali la FSM del server rimane in stato "DONE"
#define SRV_ERROR_DELAY	   2000L   // millisecondi per i quali la FSM del server rimane in stato "ERROR"

// Definizioni degli eventi da inviare al gestore del server
#define SRV_EV_NONE             0 // nessuna azione
#define SRV_EV_SEND_ONLY		1 // invia messaggio senza risposta
#define SRV_EV_SEND_AND_WAIT    2 // invia messa
#define SRV_EV_REST				3


enum SRV_MSG_ID {
	SRV_MSGID_ACCESA = 1,
	SRV_MSGID_APERTA,
	SRV_MSGID_CHIUSA,
	SRV_MSGID_ANAG,
	SRV_MSGID_RICHIESTA_CONFIG,
	SRV_MSGID_PCTIME,
	SRV_MSGID_CHECK_CFG,
	SRV_MSGID_RELOAD,
	SRV_MSGID_REFILL,
	SRV_MSGID_RICARICATO,
	SRV_MSGID_BADGE,
	SRV_MSGID_AUTORIZZO,
	SRV_MSGID_TRANSAZIONE,
	SRV_MSGID_PIATTO,
	SRV_MSGID_RESO,
	SRV_MSGID_ANNULLATO,
	SRV_MSGID_SEND_OFFLINE
};

typedef enum SRV_MSG_ID server_msg_id;



// codici di ritorno della funzione reade_anwer
enum SRV_ANSWER_CODE {
	SRV_ANSW_OK = 1,     // risposta corretta
    SRV_ANSW_DATA_ERR,   // errore nei dati in risposta
	SRV_ANSW_UNKONW,     // errore di sistema
	SRV_ANSW_INVALID_MSGID // id non previsto
};
typedef enum SRV_ANSWER_CODE server_answer_code;

// Definizioni costanti per gestione server
#define SRV_SEND_DELAY		  500
#define SRV_ANSW_WAIT		 1000
#define SRV_ANSW_TIMEOUT	 3000

#define SERVER_PAYLOAD 150

// dimensione buffer messaggi TX
#define SERVER_MAX_MESSAGE 100

_EXTVAR char usr_message_payload[SERVER_PAYLOAD]; // buffer dati utente
_EXTVAR SRV_MSG_ID usr_msg_id;                     // tipo di messaggio da inviare

void Server_init(void);
void Server_request_hand(void);
BOOL Server_request_send(uint8_t event, SRV_MSG_ID prm_msg_id, char* message);
void server_set_connection_param(void);
BOOL server_LAN_connection(void);
void Ethernet_print_info(void);											// Stampa su seriale di debug le informazioni della connessione Ethernet
event_hand_retval Server_get_event_hand_state(void);

#undef _EXTVAR