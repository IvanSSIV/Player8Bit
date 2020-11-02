// raggruppa costanti e strutture dati relative alla gestione del server

#define SERVER_TX_SIZE 100

#define SERVER_RX_LINE_SIZE 40

enum SRV_MSG_ID 
  {
     SRV_MSGID_NONE = 0,
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



// codici di ritorno della funzione read_answer
enum SRV_ANSWER_CODE 
  {
     SRV_ANSW_OK = 1,     // risposta corretta
     SRV_ANSW_DATA_ERR,   // errore nei dati in risposta
     SRV_ANSW_UNKONW,     // errore di sistema
     SRV_ANSW_INVALID_MSGID // id non previsto
  };
typedef enum SRV_ANSWER_CODE server_answer_code;

// definizione struttura globale con tutte le variabili 
// di lavoro legate alla gestione del server
struct server_work_data
  {
     BOOL          init_payload;                       // segnalato inizio payload
     uint16_t      rx_index;                           // indice per memorizzare dati da server
     char          rx_line[SERVER_RX_LINE_SIZE];       // dati riga corrente 
     char          last_rx_data[SERVER_RX_LINE_SIZE];  // dati ultima riga dati ricevuta
     uint8_t       paydec_state;       // macchina a stati riconoscimento inizio paylaod
     BOOL          ena_header_print;
     server_msg_id usr_msg_id;         // id messaggio da inviare
     uint8_t       usr_msg_type;       // tipo messaggio (solo invio/con risposta)
     BOOL          LAN_connection_OK;  // connessione LAN ok
     uint32_t      open_ms_start;      // timestamp inizio apertura connessione
     uint32_t      open_ms_end;        // timestamp fine apertura connessione  
  };
typedef struct server_work_data SERV_WORK;

