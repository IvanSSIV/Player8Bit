// include comune librerie LANZI
#include "../Lanzi_library.h"



/*----------------------------------------------------*/
/* function anag_save_start()                         */
/* Avvia la memorizzazione del file anagrafica.dat    */
/*----------------------------------------------------*/
void anag_save_start(char *rx_line)
  {
     // vedi se visualizzazione abilitata
     #ifdef ENA_PRINT_SERVER_READ_ANAG
     char buff[80];
     sprintf(buff,"save_start   -> %s",rx_line);
     debug_print_timestamp_ident(TRUE,4,buff);       
     #endif
       
     // apri il file di anagrafica
     #ifdef ANAG_SAVE_TO_SD
     anag_SD_openwr();
     #endif
  }
  
/*----------------------------------------------------*/
/* function anag_save_line()                          */
/* Memorizza una riga nel file anagrafica.dat         */
/*----------------------------------------------------*/
void anag_save_line(char *rx_line)
  {
     // vedi se visualizzazione abilitata
     #ifdef ENA_PRINT_SERVER_READ_ANAG
     char buff[80];
     static uint16_t count;
     
     count++;
     sprintf(buff,"save_line:%04u -> %s",count,rx_line);
     debug_print_timestamp_ident(TRUE,4,buff);
     #endif          
       
     // scrivi nel file
     #ifdef CONFIG_SAVE_TO_SD
     anag_SD_writeline(rx_line);
     #endif
  }
  
/*----------------------------------------------------*/
/* function anag_save_start()                         */
/* Termina la memorizzazione del file anagrafica.dat  */
/*----------------------------------------------------*/
void anag_save_end(char *rx_line)
  {
     // vedi se visualizzazione abilitata
     #ifdef ENA_PRINT_SERVER_READ_ANAG
     char buff[80];
     sprintf(buff,"save_end    -> %s",rx_line);
     debug_print_timestamp_ident(TRUE,4,buff);
     #endif
    
     // chiudi il file
     #ifdef ANAG_SAVE_TO_SD
     anag_SD_close();
     #endif
  }    
 