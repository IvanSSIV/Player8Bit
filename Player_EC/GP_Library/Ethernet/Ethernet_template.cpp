/****************************************************************************/
/* File: Ethernet_template.cpp                                              */
/* Data creazione: 06/04/2020                                                        */
/* Creato da: Ivan De Stefani                                                              */
/* Funzione: Gestione a basso livello della connessione al server tramite   */
/*                LAN controller Wiznet W5100.                                             */
/* Changelog:                                                                                */
/*                                                                                               */
/*                                                                                               */
/****************************************************************************/
// include comuni in libreria "GP_library"
#include "../GP_library.h"


IPAddress IP(0, 0, 0, 0);                     // IP della macchina
IPAddress SM(0, 0, 0, 0);                     // Maschera di sottorete
IPAddress GW(0, 0, 0, 0);                     // Gateway
IPAddress DNS(0, 0, 0, 0);                     // DNS
byte MAC[] = { 0x1E, 0x20, 0x1E, 0x1F, 0x24, 0x20 };  // Mac address della scheda
BOOL DHCP = FALSE;                                    // Flag che indica se la macchina è connessa in DHCP oppure con IP statico

// Stringhe costanti per stampa debug su seriale
const char str_dbg_w5100_init_start[] PROGMEM    = { "- Inizializzo Wiznet W5100 -" };
const char str_dbg_w5100_init_end[] PROGMEM      = { "- Fine inizializzazione Wiznet W5100 -" };
const char str_dbg_w5100_connect_fixip[] PROGMEM = { "Connetto con IP statico ..." };
const char str_dbg_w51000_MAC_address[] PROGMEM  = { "MAC Address       : %02X-%02X-%02X-%02X-%02X-%02X" };
const char str_dbg_w5100_connect_DHCP[] PROGMEM  = { "Connetto con DHCP ..." };
const char str_dbg_w5100_tempo_conne[] PROGMEM   = { "Tempo connessione : %06lu" };
const char str_dbg_w5100_indirizzo_IP[] PROGMEM  = { "Indirizzo IP : %03d.%03d.%03d.%03d" };
const char str_dbg_w5100_subnet_mask[] PROGMEM   = { "SubNetMask   : %03d.%03d.%03d.%03d" };
const char str_dbg_w5100_gateway[] PROGMEM       = { "Gateway      : %03d.%03d.%03d.%03d" };
const char str_dbg_w5100_DNSserver[] PROGMEM     = { "DNS_server   : %03d.%03d.%03d.%03d" };

//          DNS = Ethernet.dnsServerIP();
//          GW = Ethernet.gatewayIP();
//          SM = Ethernet.subnetMask();

//     sprintf(buff2,"%03d.%03d.%03d.%03d", IP[0], IP[1], IP[2], IP[3]);
//     debug_print_timestamp_title(DBG_ALWAYS_ON, 3, (char*)"Indirizzo IP acquisito : ", buff2);
const char str_dbg_w5100_mantain_DHCP[] PROGMEM  = { "DHCP Renew: " };
const char str_dbg_w5100_no_act[] PROGMEM        = { "Nothing happened." };
const char str_dbg_w5100_renew_KO[] PROGMEM      = { "Renew failed." };
const char str_dbg_w5100_renew_OK[] PROGMEM      = { "Renew succeeded." };
const char str_dbg_w5100_rebind_KO[] PROGMEM     = { "Rebind failed." };
const char str_dbg_w5100_rebind_OK[] PROGMEM     = { "Rebind succeeded." };
const char str_dbg_w5100_ERROR[] PROGMEM         = { "ERROR" };

// Risposte ricevute dal server
const char str_answ_OK1[] PROGMEM          = { "OK" };
const char str_answ_OK2[] PROGMEM          = { "$%#OK" };
const char str_answ_OK3[] PROGMEM          = { "#OK" };
const char str_answ_OK4[] PROGMEM          = { "%#OK" };
const char str_answ_RIC_REG[] PROGMEM      = { "RISP_RICARICA_REGOLARE" };
const char str_answ_BDG_SCON[] PROGMEM     = { "RISP_BADGE_SCONOSCIUTO" };
const char str_answ_BDG_NON_ATT[] PROGMEM  = { "RISP_BADGE_NON_ANCORA_ATTIVATO" };
const char str_answ_BDG_CESS[] PROGMEM     = { "RISP_BADGE_CESSATO" };
const char str_answ_BDG_NON_ABIL[] PROGMEM = { "RISP_BADGE_NON_ABILITATO" };
const char str_answ_BDG_CAR[] PROGMEM      = { "RISP_BADGE_CARICATORE_" };
const char str_answ_BDG_ABIL[] PROGMEM     = { "RISP_BADGE_ABILITATO_" };
const char str_answ_PREL_AUT[] PROGMEM     = { "RISP_PRELIEVO_AUTORIZZATO" };
const char str_answ_PREL_NON_AUT[] PROGMEM = { "RISP_PRELIEVO_NON_AUTORIZZATO" };
const char str_answ_TRANSAZ[] PROGMEM      = { "RISP_TRANSAZIONE" };
const char str_answ_LIM_MAX[] PROGMEM      = { "RISP_LIMITE_MAX_RAGGIUNTO" };
const char str_answ_PREL_RIT[] PROGMEM     = { "RISP_PRELIEVO_IN_RITARDO" };
const char str_answ_PREL_REG[] PROGMEM     = { "RISP_PRELIEVO_REGOLARE" };
const char str_answ_RESO_REG[] PROGMEM     = { "RISP_RESO_REGOLARE" };
const char str_answ_PREL_ANN[] PROGMEM     = { "RISP_PRELIEVO_ANNULLATO" };

const char* const serverAnsw[] =
  {
     str_answ_OK1,
     str_answ_OK2,
     str_answ_OK3,
     str_answ_OK4,
     str_answ_RIC_REG,
     str_answ_BDG_SCON,
     str_answ_BDG_NON_ATT,
     str_answ_BDG_CESS,
     str_answ_BDG_NON_ABIL,
     str_answ_BDG_CAR,
     str_answ_BDG_ABIL,
     str_answ_PREL_AUT,
     str_answ_PREL_NON_AUT,
     str_answ_TRANSAZ,
     str_answ_LIM_MAX,
     str_answ_PREL_RIT,
     str_answ_PREL_REG,
     str_answ_RESO_REG,
     str_answ_PREL_ANN
  };

const char str_msg_ACCESA[]       PROGMEM = { "ACCESA_" };
const char str_msg_ANAG[]         PROGMEM = { "RICHIESTA_ANAG_" };
const char str_msg_ANNULLATO[]    PROGMEM = { "ANNULLATO_" };
const char str_msg_APERTA[]       PROGMEM = { "APERTA_" };
const char str_msg_AUTORIZZO[]    PROGMEM = { "AUTORIZZO_" };
const char str_msg_BADGE[]        PROGMEM = { "BADGE_" };
const char str_msg_CHECK_CONFIG[] PROGMEM = { "CHECK_CONFIG_" };
const char str_msg_CHIUSA[] PROGMEM = { "CHIUSA_" };
const char str_msg_PIATTO[] PROGMEM = { "PIATTO_" };
const char str_msg_REFILL[] PROGMEM = { "REFILL_" };
const char str_msg_RELOAD[] PROGMEM = { "RELOAD_" };
const char str_msg_REQUEST_CONFIG[] PROGMEM = { "RICHIESTA_CONFIG_" };
const char str_msg_RESO[] PROGMEM = { "RESO_" };
const char str_msg_RICARICATO[] PROGMEM = { "RICARICATO_" };
const char str_msg_TRANSAZIONE[] PROGMEM = { "TRANSAZIONE_" };

// Messaggi da inviare al server
const char* const serverMsg[] PROGMEM = {
     str_msg_ACCESA,
     str_msg_ANAG,
     str_msg_ANNULLATO,
     str_msg_APERTA,
     str_msg_AUTORIZZO,
     str_msg_BADGE,
     str_msg_CHECK_CONFIG,
     str_msg_CHIUSA,
     str_msg_PIATTO,
     str_msg_REFILL,
     str_msg_RELOAD,
     str_msg_REQUEST_CONFIG,
     str_msg_RESO,
     str_msg_RICARICATO,
     str_msg_TRANSAZIONE
};

// Define locali al modulo
#define ETHERNET_DHCP_OK    1  // connessione DCHP ok
#define ETHERNET_DHCP_FAIL  0  // errore DHCP
#define ETHERNET_FIXIP_OK   3  // connessione IP fisso ok
#define ETHERNET_FIXIP_FAIL 4  // timeout connessione IP fisso

#define MAX_CONNECT_TIME   59000L 
#define ETHERNET_MAX_RETRY  3

#define MAX_SRV_ANSW_SIZE          31

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
     char buff[80];
     
     debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_dbg_w5100_init_start);

     IP[0] = 0;
     IP[1] = 0;
     IP[2] = 0;
     IP[3] = 0;
     
     IPAddress loc_IP;
     uint32_t start_ms, end_ms, connect_time;

     debug_message_ident_push();

     start_ms = RTOS_get_tick_1ms();
     if (IP[0] == 0 && IP[1] == 0 && IP[2] == 0 && IP[3] == 0)
       {
          // visualizza "connetto con DHCP"
          debug_message_timestamp_PGM(str_dbg_w5100_connect_DHCP);
          debug_message_ident_push();
          sprintf(buff,AVR_PGM_to_str(str_dbg_w51000_MAC_address),
                  MAC[0],
                  MAC[1],
                  MAC[2],
                  MAC[3],
                  MAC[4],
                  MAC[5]);
          debug_message_timestamp(buff);
          
          // inizializza ehternet con DCHP
          Ethernet.begin(MAC);
          end_ms = RTOS_get_tick_1ms();
          connect_time = end_ms - start_ms;
          DHCP = TRUE;

          IP = Ethernet.localIP();
          DNS = Ethernet.dnsServerIP();
          GW = Ethernet.gatewayIP();
          SM = Ethernet.subnetMask();

          sprintf(buff, AVR_PGM_to_str(str_dbg_w5100_tempo_conne), connect_time);
          debug_message_timestamp(buff);

          debug_message_ident_pop();
       }
     else
       {
          debug_message_timestamp_PGM(str_dbg_w5100_connect_fixip);
          DHCP = FALSE;
          // inizializza ehternet con IP statico
          Ethernet.begin(MAC, IP);

          end_ms = RTOS_get_tick_1ms();
          connect_time = end_ms - start_ms;
       }

     // visualizza configurazione indirizzi IP
     Ethernet_print_address();

     // Numero di ritrasmissioni della comunicazione e intervallo fra di esse (3 tentativi ad 1 s l'uno dall'altro)
     W5100.setRetransmissionTime(1000);
     W5100.setRetransmissionCount(3);

     debug_message_ident_pop();
     debug_message_timestamp_PGM(str_dbg_w5100_init_end);
  }

//const char str_dbg_w5100_indirizzo_IP[] PROGMEM  = { "Indirizzo IP : %03d.%03d.%03d.%03d" };
//const char str_dbg_w5100_subnet_mask[] PROGMEM   = { "SubNetMask   : %03d.%03d.%03d.%03d" };
//const char str_dbg_w5100_gateway[] PROGMEM       = { "Gateway      : %03d.%03d.%03d.%03d" };
//const char str_dbg_w5100_DNSserver[] PROGMEM     = { "DNS_server   : %03d.%03d.%03d.%03d" };

void Ethernet_print_address(void) 
  {
     char buff[80];

     sprintf(buff,AVR_PGM_to_str(str_dbg_w5100_indirizzo_IP),IP[0], IP[1], IP[2], IP[3]);
     debug_message_timestamp(buff);

     sprintf(buff,AVR_PGM_to_str(str_dbg_w5100_subnet_mask),SM[0], SM[1], SM[2], SM[3]);
     debug_message_timestamp(buff);

     sprintf(buff,AVR_PGM_to_str(str_dbg_w5100_gateway), GW[0], GW[1], GW[2], GW[3]);
     debug_message_timestamp(buff);

     sprintf(buff,AVR_PGM_to_str(str_dbg_w5100_DNSserver), DNS[0], DNS[1], DNS[2], DNS[3]);
     debug_message_timestamp(buff);
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
               strncat(buff, AVR_PGM_to_str(str_dbg_w5100_no_act), sizeof(buff) - 1);              // Niente
               break;
          case 1:
               strncat(buff, AVR_PGM_to_str(str_dbg_w5100_renew_KO), sizeof(buff) - 1);               // Fallito il rinnovamento del lease DHCP
               break;
          case 2:
               strncat(buff, AVR_PGM_to_str(str_dbg_w5100_renew_OK), sizeof(buff) - 1);              // Rinnovamento del lease DHCP andato a buon fine
               break;
          case 3:
               strncat(buff, AVR_PGM_to_str(str_dbg_w5100_rebind_KO), sizeof(buff) - 1);               // Hard-reset del DHCP fallito
               break;
          case 4:
               strncat(buff, AVR_PGM_to_str(str_dbg_w5100_rebind_OK), sizeof(buff) - 1);              // Hard-reset del DHCP andato a buon fine
               break;

          default:
               strncat(buff, AVR_PGM_to_str(str_dbg_w5100_ERROR), sizeof(buff) - 1);                   // Errore nella gestione del lease DHCP
               break;
     }
     #ifdef DEBUG_VERSION
          debug_print_timestamp_ident(eth_dbg, DEBUG_IDENT_L2, buff);
     #endif
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
     strncpy(dest, AVR_PGM_to_str(serverMsg[index]), sizeof(dest) - 1);
     strncat(dest, message, sizeof(dest) - 1);
}
