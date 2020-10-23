#define MODULE_CONFIG_LIB

// include comune librerie LANZI
#include "../Lanzi_library.h"

// prototipi funzione locali
static void config_data_init(void);

/*------------------------------------------------------*/
/* function config_init_all()                           */
/* Raggruppa le attivita legate al config da richiamare */
/* durante la init                                      */
/*------------------------------------------------------*/
void config_init_all(void)
  {
     // inizializza struttura dati gestione config
     config_data_init();
     // inizializza modulo lettura config da LAN
     config_LAN_read_init();
     // inizializza modulo lettura config da SD
     config_SD_read_init();
     // inzializz modulo lettura config da EEPROM
     config_EEPROM_read_init();
  }

/*------------------------------------------------------*/
/* function config_main_all()                           */
/* Raggruppa le attivita legate al config da richiamare */
/* a livello del main                                   */
/*------------------------------------------------------*/
void config_main_all(void)
  {
     // richiama gestione lettura da LAN
     config_LAN_read_hand();
     // richiama gestione lettura da SD
     config_SD_read_hand();
     // richiama gestione lettura da EEPROM
     config_EEPROM_read_hand();

  }

BOOL verifica_congruita_config_ricevuto(void)
  {
	return TRUE;
  }

/*-------------------------------------------------------------------------*/
/* Inizializza la struttura di lavoro con le variabili per                 */
/* la gestione del config , da non confondere con il config vero e proprio */
/*-------------------------------------------------------------------------*/
static void config_data_init(void)
  {
     memset(&config_work, 0, sizeof(config_work));
     config_work.dbg_ena = TRUE;
     config_work.dbg_idn = 2;
  }


#undef MODULE_CONFIG_LIB