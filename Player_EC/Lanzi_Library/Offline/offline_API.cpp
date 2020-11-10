/****************************************************************************/
/* Modulo per la gestione dei file offline, utilizzati per la gestione      */
/* della comunicazione al server delle transazioni avvenute quando la       */
/* macchina era fuori rete.                                                 */
/* La sintassi dei file offline è sempre comune, hanno un header che indica */
/* data e ora della transazione e riportano il messaggio che sarebbe stato  */
/* inviato al server nel caso in cui la macchina fosse stata online.        */
/****************************************************************************/

#include "../Lanzi_library.h"
#include "SD.h"

static File offline_file;

// Struttura del messaggio offline:
// OFFLINE_gg/mm/aaaa hh:mm:ss - [messaggio]
const char str_offline_header[] PROGMEM = { "OFFLINE_" };

void create_offline_file(char* msg)
{
  char buff[120];
  char filename[15];
  char datetime[60];
  get_datetime_ASCII_filedat(datetime, sizeof(datetime));
  sprintf(buff, "%s - %s", datetime, msg);
  get_datetime_ASCII_Lanzi(filename, sizeof(filename));
  strncat(filename, (char*)".OFF", sizeof(filename) - 1);
  offline_file = SD.open(filename, FILE_WRITE);
  offline_file.println(buff);
  offline_file.close();
  
}