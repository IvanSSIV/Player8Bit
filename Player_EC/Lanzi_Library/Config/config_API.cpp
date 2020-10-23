/*********************************************************************************/
/* Questo modulo richiede -per la sua operatività- che a livello di applicazione */
/* stia stata effettuata l'inizializzazione a basso livello della gestione della */
/* SD.                                                                           */
/* Ad esempio, la SD.begin va inserita a livello della INIT globale, nei driver  */
/* hardware che si occupano della gestione a basso livello della scheda SD.      */
/*********************************************************************************/
// include comune librerie LANZI
#include "../Lanzi_library.h"
#include "SD.h"

// variabili locali
#if CONFIG_MEDIA == MEDIA_SD
static File config_file_wr;
static File config_file_rd;
#endif

// Modulo di interfaccia tra la gestione del CONFIG (astratta) e le funzioni
// per la scrittura del CONFIG su SD. È l'unico modulo della gestione del
// CONFIG legato alla gestione hardware e va eventualmente modificato
// nel caso si scelga un supporto di memorizzazione diverso.

/*--------------------------------------------------*/
/* function config_openwr()                         */
/* Apre in Scrittura il file del config             */
/* Attualmente restituisce TRUE ma vanno inseriti i */
/* controlli di presenza file e presenza SD , ecc   */
/*--------------------------------------------------*/
BOOL config_openwr(void)
  {
     #if CONFIG_MEDIA == MEDIA_SD
     SD.remove(NOME_FILE_CONFIG);
     config_file_wr = SD.open(NOME_FILE_CONFIG, FILE_WRITE);
     #else
     return TRUE;
     #endif
  }

void config_closewr(void)
  {
     #if CONFIG_MEDIA == MEDIA_SD
     config_file_wr.close();
     #else
     #endif
  }

void config_writeline(char *line)
  {
     #if CONFIG_MEDIA == MEDIA_SD
     config_file_wr.println(line);
     #else
     #endif
  }

/*--------------------------------------------------*/
/* function config_SD_openrd()                      */
/* Apre in lettura il file del config               */
/*--------------------------------------------------*/
void config_openrd(char *filename)
  {
     config_file_rd = SD.open(filename);
  }

/*--------------------------------------------------*/
/* function config_file_exist()                     */
/* Attualmente restituisce TRUE ma vanno inseriti i */
/* controlli di presenza file e presenza SD , ecc   */
BOOL config_file_exist(char *filename)
  {
     #if CONFIG_MEDIA == MEDIA_SD
     if (SD.exists(filename))
       return TRUE;
     else
       return FALSE;
     #else
     return FALSE;
     #endif
  }

int config_char_available(void)
  {
     #if CONFIG_MEDIA == MEDIA_SD
     return (config_file_rd.available());
     #else
     return 0;
     #endif
  }

int config_read_char(void)
  {
     #if CONFIG_MEDIA == MEDIA_SD
     return (config_file_rd.read());
     #else
     return 0;
     #endif
  }

void config_closerd(void)
  {
     #if CONFIG_MEDIA == MEDIA_SD
     config_file_rd.close();
     #else
     #endif
  }
