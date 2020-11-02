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



char* sdreadbuff;
VM_DATA vm_data;

const char appl_init_start[] PROGMEM = { "Inizio appl_init" };
const char appl_init_end[] PROGMEM   = { "Fine appl_init" };

void appl_init(void)
  {
    char tmpbuff[80];
     // inizializza seriale di debug
     debug_serial_init();
     serial_debug_init();
     // visualizza titolo applicazione
     appl_init_title();

     debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(appl_init_start));

     // inzializza GPIO
     GPIO_init();

     // inizializza HW debug
     dbg_HW_init();

     // inzializza LCD
     LCD_init();

     // inizializza il RTC
     RTC_init_LL();

     // Inizializza la scheda SD
     // SD_init();
     
     // inizializza gestione PLC e logica macchina
     appl_test_PLC_init();

     basetempi_RTOS_init();



     // inizializza il modulo w5100
     Ethernet_init();
     // inizializza la gestione del server
     Server_init();
     // Inizializza la memoria EEPROM
     EEPROM_Init();
     // Inizializza la gestione del motore
     motor_init();
     // Prova invio messaggio di reportistica errori
     send_error_report(0);

     // Inizializzazione debug per gestione attivazione da seriale

     debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(appl_init_end));
     debug_print_CRLF(DBG_ALWAYS_ON);

     //RTC_ena_print();
  }