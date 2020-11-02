/****************************************************************************/
/* File: Ethernet_template.cpp                                              */
/* Data creazione: 06/04/2020           									*/
/* Creato da: Ivan De Stefani  												*/
/* Funzione: Gestione a basso livello della connessione al server tramite   */
/*			 LAN controller Wiznet W5100.									*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

// Include delle librerie
// Include librerie W5100
#include "SPI.h"
#include <Ethernet.h>
#include <utility/w5100.h>
#include "Arduino.h"
#include "IPAddress.h"

#include "project_def.h"
#include "debug_opzion.h"

#include "My_Include/mydef.h"

#include "My_Debug/Debug_Serial/debug_serial.h"
#include "My_Debug/Debug_Utility/debug_utility.h"
#include "My_Debug/Serial_Command/serial_command.h"
#include "My_Library/Utilities/AVR_API.h"
#include "Hardware/CPU_core/timing.h"

#include "Ethernet_template.h"

IPAddress IP(0, 0, 0, 0);								// IP della macchina
IPAddress SM(0, 0, 0, 0);								// Maschera di sottorete
IPAddress GW(0, 0, 0, 0);								// Gateway
IPAddress DNS(0, 0, 0, 0);								// DNS
byte MAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };		// Mac address della scheda
BOOL DHCP = FALSE;										// Flag che indica se la macchina è connessa in DHCP oppure con IP statico

// Stringhe costanti per stampa debug su seriale
const char str_dbg_w5100_init_start[] PROGMEM    = { "Inizializzo Wiznet W5100 ..." };
const char str_dbg_w5100_init_end[] PROGMEM      = { "Fine inizializzazione Wiznet W5100." };
const char str_dbg_w5100_connect_fixip[] PROGMEM = { "Connetto con IP statico ..." };
const char str_dbg_w5100_connect_DHCP[] PROGMEM  = { "Connetto con DHCP ..." };
const char str_dbg_w5100_mantain_DHCP[] PROGMEM  = { "DHCP Renew: " };
const char str_dbg_w5100_no_act[] PROGMEM        = { "Nothing happened." };
const char str_dbg_w5100_renew_KO[] PROGMEM      = { "Renew failed." };
const char str_dbg_w5100_renew_OK[] PROGMEM      = { "Renew succeeded." };
const char str_dbg_w5100_rebind_KO[] PROGMEM     = { "Rebind failed." };
const char str_dbg_w5100_rebind_OK[] PROGMEM     = { "Rebind succeeded." };
const char str_dbg_w5100_ERROR[] PROGMEM         = { "ERROR" };

// Risposte ricevute dal server

char* serverAnsw[] =
{
	"OK",
	"$%#OK",
	"#OK",
	"%#OK",
	"RISP_RICARICA_REGOLARE",
	"RISP_BADGE_SCONOSCIUTO",
	"RISP_BADGE_NON_ANCORA_ATTIVATO",
	"RISP_BADGE_CESSATO",
	"RISP_BADGE_NON_ABILITATO",
	"RISP_BADGE_CARICATORE_",
	"RISP_BADGE_ABILITATO_",
	"RISP_PRELIEVO_AUTORIZZATO",
	"RISP_PRELIEVO_NON_AUTORIZZATO",
	"RISP_TRANSAZIONE",
	"RISP_LIMITE_MAX_RAGGIUNTO",
	"RISP_PRELIEVO_IN_RITARDO",
	"RISP_PRELIEVO_REGOLARE",
	"RISP_RESO_REGOLARE",
	"RISP_PRELIEVO_ANNULLATO"
};

// Messaggi da inviare al server
char* serverMsg[] = {
	"ACCESA_",
	"RICHIESTA_ANAG_",
	"ANNULLATO_",
	"APERTA_",
	"AUTORIZZO_",
	"BADGE_",
	"CHECK_CONFIG_",
	"CHIUSA_",
	"PIATTO_",
	"REFILL_",
	"RELOAD_",
	"RICHIESTA_CONFIG_",
	"RESO_",
	"RICARICATO_",
	"TRANSAZIONE_"
};

// Define locali al modulo
#define ETHERNET_DHCP_OK    1  // connessione DCHP ok
#define ETHERNET_DHCP_FAIL  0  // errore DHCP
#define ETHERNET_FIXIP_OK   3  // connessione IP fisso ok
#define ETHERNET_FIXIP_FAIL 4  // timeout connessione IP fisso

#define MAX_CONNECT_TIME   59000L 
#define ETHERNET_MAX_RETRY  3

#define MAX_SRV_ANSW_SIZE		31

// Prototipi funzione locali
BOOL starts_with(char* message, char* constant);

/***********************************************/
/* function Ethernet_init()                    */
/* Inizializza la connessione ethernet in DHCP */
/* o con IP statico, a seconda del file CONFIG */
/***********************************************/
void Ethernet_init(void)
{
	uint8_t i;
	debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_w5100_init_start));

	//IP[0] = 192;
	//IP[1] = 168;
	//IP[2] = 137;
	//IP[3] = 14;
	
	IPAddress loc_IP;
	uint32_t start_ms, end_ms, connect_time;

	start_ms = RTOS_get_tick_1ms();
	if (IP[0] == 0 && IP[1] == 0 && IP[2] == 0 && IP[3] == 0)
	{
		debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_w5100_connect_DHCP));
		// inizializza ehternet con DCHP
		Ethernet.begin(MAC);
		end_ms = RTOS_get_tick_1ms();
		connect_time = end_ms - start_ms;
		DHCP = TRUE;
		IP = Ethernet.localIP();
		DNS = Ethernet.dnsServerIP();
		GW = Ethernet.gatewayIP();
		SM = Ethernet.subnetMask();
	}
	else
	{
		debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_w5100_connect_fixip));
		DHCP = FALSE;
		// inizializza ehternet con IP statico
		Ethernet.begin(MAC, IP);


		end_ms = RTOS_get_tick_1ms();
		connect_time = end_ms - start_ms;
	}

	char buff[80];
	sprintf(buff, "Tempo connessione: %06lu", connect_time);
	debug_print_timestamp(DBG_ALWAYS_ON, (char*)buff);
	
	// Numero di ritrasmissioni della comunicazione e intervallo fra di esse (3 tentativi ad 1 s l'uno dall'altro)
	W5100.setRetransmissionTime(1000);
	W5100.setRetransmissionCount(3);

	debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_w5100_init_end));
}

/*************************************************/
/* function Ethernet_mantain_lease()             */
/* Mantiene attiva la connessione a internet per */
/* evitare problemi con lease del DHCP server.   */
/*************************************************/
void Ethernet_mantain_lease(void)
{
	static int ETH_retval;
	char buff[80];
	ETH_retval = Ethernet.maintain();
	memset(buff, 0, sizeof(buff));
	strncpy(buff, AVR_PGM_to_str(str_dbg_w5100_mantain_DHCP), sizeof(buff) - 1);
	switch (ETH_retval)
	{
		case 0:
			strncat(buff, AVR_PGM_to_str(str_dbg_w5100_no_act), sizeof(buff) - 1);		    // Niente
			break;
		case 1:
			strncat(buff, AVR_PGM_to_str(str_dbg_w5100_renew_KO), sizeof(buff) - 1);			// Fallito il rinnovamento del lease DHCP
			break;
		case 2:
			strncat(buff, AVR_PGM_to_str(str_dbg_w5100_renew_OK), sizeof(buff) - 1);		    // Rinnovamento del lease DHCP andato a buon fine
			break;
		case 3:
			strncat(buff, AVR_PGM_to_str(str_dbg_w5100_rebind_KO), sizeof(buff) - 1);			// Hard-reset del DHCP fallito
			break;
		case 4:
			strncat(buff, AVR_PGM_to_str(str_dbg_w5100_rebind_OK), sizeof(buff) - 1);		    // Hard-reset del DHCP andato a buon fine
			break;

		default:
			strncat(buff, AVR_PGM_to_str(str_dbg_w5100_ERROR), sizeof(buff) - 1);			    // Errore nella gestione del lease DHCP
			break;
	}
	debug_print_timestamp_ident(eth_dbg, DEBUG_IDENT_L2, buff);
}

/*****************************************************/
/* function starts_with()                            */
/* Verifica se la risposta ricevuta dal server è     */
/* uno dei messaggi preimpostati.                    */
/*****************************************************/
BOOL starts_with(char* message, char* constant)
{
	return(strncmp(message, constant, strnlen(constant, MAX_SRV_ANSW_SIZE)) == 0);
}

/*****************************************************/
/* function Ethernet_build_message()                 */
/* Crea il messaggio da inviare al server.           */
/*****************************************************/
void Ethernet_build_message(char* dest, uint8_t index, char* message)
{
	strncpy(dest, serverMsg[index], sizeof(dest) - 1);
	strncat(dest, message, sizeof(dest) - 1);
}
