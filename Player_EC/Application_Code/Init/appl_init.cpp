/****************************************************************************/
/* File: appl_init.cpp                                                      */
/* Data creazione: 17/03/2020												*/
/* Creato da: Adriano Olivetti - Ivan De Stefani        					*/
/* Funzione: Inizializzazione dei moduli alla base del codice macchina.     */
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

#include "player_EC.h"


char* sdreadbuff;
VM_DATA vm_data;

// Prototipi funzione locale
//static void init_uso_memoria_RAM(void);

const char appl_init_start[] PROGMEM = { "## inizio appl_init() ##" };
const char appl_init_end[] PROGMEM   = { "## fine appl_init() ##" };

void appl_init(void)
  {
     char tmpbuff[80];

     // inizializza seriale di debug
     debug_serial_init();
     // inizializza le abilitaziondi del debug
     serial_debug_init();
     // inizializza HW debug
     dbg_HW_init();

     // inizializza gestione messaggi di debug
     debug_message_init();

     // visualizza titolo applicazione

     // inizializza variabili globali
     global_data_init();

     appl_init_title();
     debug_message_timestamp_PGM(appl_init_start);
 
     // inzializza GPIO
     GPIO_init();

     // Inizializza ingressi ad alta velocità
    // hi_speed_init_LL();

     // inzializza LCD
     LCD_init();

     // inizializza il RTC
     RTC_init_LL();

     // Inizializza la scheda SD
     SD_init();
     
     // inizializza gestione PLC e logica macchina
     //appl_test_PLC_init();

     logic_init();

     basetempi_RTOS_init();

     // inizializza il modulo w5100
     Ethernet_init();

     // inizializza la gestione del server
     server_init();
     
     // Inizializza la memoria EEPROM
     EEPROM_Init();

     // Inizializza la gestione del motore
     motor_init();
     
     // Prova invio messaggio di reportistica errori
     //send_error_report(0);
     
     // Inizializzazione modulo di gestione dei badge
     badge_init();

     // Inizializzazione del modulo di gestione della logica
     logic_init();
     
     // Inizializzazione modulo di gestione erogazione
     erog_init();

     // inizializza gestione config
     config_init_all();

     // inizializza gestione anagrafica
     //anag_init_all();

     storage_init();
 
     debug_message_enable(TRUE,1);
     debug_message_CRLF();
     debug_message_timestamp_PGM(appl_init_end); 
     debug_print_CRLF(DBG_ALWAYS_ON);

     //for (;;);

    // init_uso_memoria_RAM();
    // config_test();
  }


/*
static void init_uso_memoria_RAM(void) 
  {
     char buff[80];
     sprintf(buff, "Dimensione config: %03u", sizeof(setup_macchina));
     debug_print_timestamp(DBG_ALWAYS_ON, buff);
     sprintf(buff, "Dimensione dati server: %03u", sizeof(SERV_WORK));
     debug_print_timestamp(DBG_ALWAYS_ON, buff);
     sprintf(buff, "Dimensione messaggi stato appl_erog %03u", erog_sizeof_state_message());
     debug_print_timestamp(DBG_ALWAYS_ON, buff);
  }
  */