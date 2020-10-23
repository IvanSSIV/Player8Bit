#ifdef MODULE_SRV_UTILS
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif 


_EXTVAR uint8_t last_server_event;
_EXTVAR short open_connect_retval;
_EXTVAR char tx_message_payload[SERVER_PAYLOAD];  // buffer dati da inviare (payload)

void server_utility_init(void);
void server_set_connection_param(void);
BOOL server_LAN_connection(void);
BOOL server_invia_messaggio_senza_risposta(void);
BOOL server_invia_messaggio_con_risposta(void);
uint8_t server_read_answer(char* answer, uint16_t answ_size);
void server_start_read(void);

BOOL server_open(void);
void server_close(void);
void server_transmit(void);
void Ethernet_print_info(void);
char* serv_ID_to_str(SRV_MSG_ID ID);
void serv_event_to_str(uint8_t event, char* msg, uint8_t size);

#undef _EXTVAR
