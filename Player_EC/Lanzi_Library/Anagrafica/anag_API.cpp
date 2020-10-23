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
static File anag_file_wr;
static File anag_file_rd;


// Modulo di interfaccia tra la gestione dell'anagrafica (astratta) e le funzioni
// per la scrittura del anagrafica su SD. È l'unico modulo della gestione del
// anagafica legato alla gestione hardware e va eventualmente modificato
// nel caso si scelga un supporto di memorizzazione diverso.

/*--------------------------------------------------*/
/* function anag_SD_openwr()                        */
/* Apre in Scrittura il file dell'anagrafica        */
/* Attualmente restituisce TRUE ma vanno inseriti i */
/* controlli di presenza file e presenza SD , ecc   */
/*--------------------------------------------------*/
BOOL anag_SD_openwr(void)
  {
     SD.remove(NOME_FILE_ANAGRAFICA);
     anag_file_wr = SD.open(NOME_FILE_ANAGRAFICA, FILE_WRITE);
  }

void anag_SD_close(void)
  {
     anag_file_wr.close();
  }

void anag_SD_writeline(char *line)
  {
     anag_file_wr.println(line);
  }


/*--------------------------------------------------*/
/* function anag_SD_openrd()                        */
/* Apre in lettura il file dell'anagrafica          */
/* Attualmente restituisce TRUE ma vanno inseriti i */
/* controlli di presenza file e presenza SD , ecc   */
/*--------------------------------------------------*/
BOOL anag_SD_openrd(void)
  {
     anag_file_rd = SD.open(NOME_FILE_ANAGRAFICA);
     return TRUE;
  }

int anag_char_available(void)
  {
     return (anag_file_rd.available());
  }

int anag_read_char(void)
  {
     return (anag_file_rd.read());
  }
