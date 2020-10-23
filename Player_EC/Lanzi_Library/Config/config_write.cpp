// include comune librerie LANZI
#include "../Lanzi_library.h"

/*-----------------------------------------------------------------------*/
/* Viene richiamata dalla server_read_answer all'arrivo della prima riga */
/* del config durante il download del config dal server                  */
/* Effettua tutte le attività di apertura per inizializzare la ricezione */
/* del config appoggiato in una struttura in memoria                     */
/* La "config_write_line" salva in tempo reale il contenuto ricevuto     */
/* nel file su SD                                                        */
/*-----------------------------------------------------------------------*/
void config_save_start(char* line)
  {
     #ifdef DEBUG_VERSION
     debug_print_timestamp_ident(config_work.dbg_ena, config_work.dbg_idn, (char*)"start : ");
     #endif

     // apri il file "NWCONFIG.DAT"
     config_openwr();
 
     // memorizza la riga 
     if (strncmp(line, "#PAY_", 5) != 0)
       {
          config_writeline(line);
       }

     // decodificane il contenuto
     config_decode_line_from_server(line, config_work.dbg_ena);
  }

/*-----------------------------------------------------------------------*/
/* Viene richiamata dalla server_read_answer all'arrivo delle successive */
/* righe del config durante il download del config dal server            */
/*-----------------------------------------------------------------------*/
void config_save_line(char* line)
  {
     #ifdef DEBUG_VERSION
     debug_print_timestamp_ident(config_work.dbg_ena, config_work.dbg_idn, (char*)"line  : ");
     #endif

     // memorizza la riga ricevuta nel "NWCONFIG.DAT" 
     if (strncmp(line, "#PAY_", 5) != 0)
       {
          config_writeline(line);
       }
 
     // decodificane il contenuto
     config_decode_line_from_server(line, config_work.dbg_ena);
  }

/*-----------------------------------------------------------------------*/
/* Viene richiamata dalla server_read_answer all'arrivo dell' ultima     */
/* riga del config durante il download del config dal server            */
/*-----------------------------------------------------------------------*/
void config_save_end(char* line)
  {
     #ifdef DEBUG_VERSION
     debug_print_timestamp_ident(config_work.dbg_ena, config_work.dbg_idn, (char*)"end   : ");
     #endif

     // memorizza la riga ricevuta nel "NWCONFIG.DAT"
     if (strncmp(line, "#PAY_", 5) != 0)
       {
          config_writeline(line);
       }
 
     // chiudi il file "NWCONFIG.DAT"
     config_closewr();

     // decodificane il contenuto
     config_decode_line_from_server(line, config_work.dbg_ena);
  }
