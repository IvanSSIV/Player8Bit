
#ifdef MODULE_ETHERNET
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

// Variabili pubbliche


// Prototipi funzione
void Ethernet_init(void);												// Inizializza a basso livello il modulo W5100
void Ethernet_mantain_lease(void);										// Rinnova il lease DHCP
void Ethernet_build_message(char* dest, uint8_t index, char* message);	// Costruisce il messaggio da mandare al server
void Ethernet_print_address(void);

// Define mnemoniche per i messaggi ricevuti dal server
#define SRV_ANSW_OK1							0
#define SRV_ANSW_OK2							1
#define SRV_ANSW_OK3							2
#define SRV_ANSW_OK4							3
#define SRV_ANSW_RICARICA_REGOLARE				4
#define SRV_ANSW_BADGE_SCONOSCIUTO				5
#define SRV_ANSW_BADGE_NON_ATTIVATO				6
#define SRV_ANSW_BADGE_CESSATO					7
#define SRV_ANSW_BADGE_NON_ABILITATO			8
#define SRV_ANSW_BADGE_CARICATORE				9
#define SRV_ANSW_BADGE_ABILITATO			   10
#define SRV_ANSW_PRELIEVO_AUTORIZZATO		   11
#define SRV_ANSW_PRELIEVO_NON_AUTORIZZATO	   12
#define SRV_ANSW_TRANSAZIONE				   13
#define SRV_ANSW_LIM_MAX					   14
#define SRV_ANSW_PRELIEVO_IN_RITARDO		   15
#define SRV_ANSW_PRELIEVO_REGOLARE			   16
#define SRV_ANSW_RESO_REGOLARE				   17
#define SRV_ANSW_PRELIEVO_ANNULLATO			   18

// Define mnemoniche per i messaggi da inviare al server
#define SRV_MSG_ACCESA							0
#define SRV_MSG_ANAG							1
#define SRV_MSG_ANNULLATO						2
#define SRV_MSG_APERTA							3
#define SRV_MSG_AUTORIZZO						4
#define SRV_MSG_BADGE							5
#define SRV_MSG_CHECK_CONFIG					6
#define SRV_MSG_CHIUSA							7
#define SRV_MSG_PIATTO							8
#define SRV_MSG_REFILL							9
#define SRV_MSG_RELOAD						   10
#define SRV_MSG_REQUEST_CONFIG				   11
#define SRV_MSG_RESO						   12
#define SRV_MSG_RICARICATO					   13
#define SRV_MSG_TRANSAZIONE					   14

#undef _EXTVAR