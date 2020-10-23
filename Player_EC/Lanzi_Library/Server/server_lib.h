#ifdef MODULE_APPL_SERVER
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

// Define per gli stati della gestione logica della comunicazione con il server
#define MAX_SRV_STATE           9

#define SRV_ST_INIT             0  // condizione iniziale
#define SRV_ST_IDLE             1  // attesa comandi
#define SRV_ST_SENDING_ONLY     2  // invio senza risposta
#define SRV_ST_SENDING          3  // invio prima della risposta
#define SRV_ST_WAITING_ANSW     4  // attesa risposta
#define SRV_ST_DONE             5  // eseguito il comando
#define SRV_ST_TIMEOUT          6  // avvenuto timeout
#define SRV_ST_ERROR            7  // errore recuperabile
#define SRV_ST_FAIL             8  // errore irrecuperabile, stato "incricched" o "pianted"

#define SRV_DONE_DELAY	    500L   // millisecondi per i quali la FSM del server rimane in stato "DONE"
#define SRV_ERROR_DELAY	   2000L   // millisecondi per i quali la FSM del server rimane in stato "ERROR"

// Definizioni degli eventi da inviare al gestore del server
#define SRV_EV_NONE             0 // nessuna azione
#define SRV_EV_SEND_ONLY        1 // invia messaggio senza risposta
#define SRV_EV_SEND_AND_WAIT    2 // invia messaggio e attende risposta
#define SRV_EV_REST             3




// Definizioni costanti per gestione server
#define SRV_SEND_DELAY           500
#define SRV_ANSW_WAIT           1000
#define SRV_ANSW_TIMEOUT        3000

#define SERVER_PAYLOAD 150

// dimensione buffer messaggi TX
#define SERVER_MAX_MESSAGE 100

_EXTVAR char      usr_message_payload[SERVER_PAYLOAD]; // buffer dati utente
_EXTVAR SERV_WORK server_work;                    // struttura lavoro per gestione server

void server_init(void);
void server_request_hand(void);
BOOL server_request_send(server_msg_id prm_msg_id, char* message);
void server_reset_error(void);
void server_set_connection_param(void);
BOOL server_LAN_connection(void);
void Ethernet_print_info(void); // Stampa su seriale di debug le informazioni della connessione Ethernet
FSM_RETVAL server_request_state(void);

#undef _EXTVAR