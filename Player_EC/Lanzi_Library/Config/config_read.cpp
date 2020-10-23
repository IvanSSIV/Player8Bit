// include comune librerie LANZI
#include "../Lanzi_library.h"

// messaggi di debug
static const char str_log_func_read_file[] PROGMEM = {"function config_read_file()"};
static const char str_log_file_notfound[] PROGMEM  = {"file config non trovato"};
static const char str_log_leggo_config[] PROGMEM   = {"leggo file di configurazione :"};

// definizione dimensione buffer di lettura
#define READ_BUFF_SIZE 80
/*---------------------------------------------------------------*/
/* function config_read_file()                                   */
/* Legge il file di configurazione dalla SD                      */
/* Restituisce TRUE se il file esiste , FALSE in caso contrario  */
/*---------------------------------------------------------------*/
BOOL config_read_file(char *filename,BOOL ena_print,BOOL ena_getdata)
  {
     char read_buff[READ_BUFF_SIZE];
     char buff[120];
     char *pos_ricarica;
     uint8_t index;
     char i;
     BOOL retval;

     // vedi se debug abilitato
     debug_message_enable(config_work.dbg_ena,config_work.dbg_idn);
     debug_message_timestamp_PGM(str_log_func_read_file);
     
     index = 0;
     memset(read_buff,0,READ_BUFF_SIZE);
     retval = FALSE;

     // verifica se file esiste
     if (config_file_exist(filename))
       {
          // visualizza nome del file
          debug_print_ena(config_work.dbg_ena,filename);
          debug_message_timestamp_PGM(str_log_leggo_config);

          // apri il file in lettura
          config_openrd(filename);

          while (config_char_available())
            {
               i = config_read_char();
               switch (i)
                 {
                    case 0x0D:
                      // vedi se visualizzazione abilitata
                      if (ena_print)
                        {
                           //sprintf(buff,"size:%02d msg:%s",strlen(read_buff),read_buff);
                           //debug_print_timestamp_ident(TRUE,6,buff);                     
                        }
                      // vedi se lettura dati abilitata
                      if (ena_getdata)
                        {
                           config_decode_line_from_SD(read_buff,ena_print);
                        }                   
                      memset(read_buff,0,READ_BUFF_SIZE);
                      index = 0;
                      break;
         
                    case 0x0A:
                      memset(read_buff,0,READ_BUFF_SIZE);
                      index = 0;
                      break;
         
                    default:
                      if (index < (READ_BUFF_SIZE-1))
                        {
                           read_buff[index] = i;
                           index++;
                        }
                 }
            }
          config_closerd();
          retval = TRUE;
       }
     else
       {
          debug_message_timestamp_PGM(str_log_file_notfound);
       }
     return retval;
  }  
