// include comune librerie LANZI
#include "../Lanzi_library.h"

/*----------------------------------------------------------------------*/
/* function anagrafica_cerca_badge_offline()                            */
/* Cerca un badge nella tabelle dei badge abilitati (file ANAG.DAT)     */
/* Riceve in ingresso una stringa di caratteri con il badge da cercare  */
/* In uscita fornisce uno dei codici di risposta ID_RESP_BADGE_xx       */
/* definiti nel file "gestione_badge."                                  */
/*----------------------------------------------------------------------*/
#define ANAG_LINE_SIZE 30
FUNC_RETVAL anagrafica_cerca_badge_offline(char *badge,uint8_t *esito_ricerca)
  {
     char c,line[ANAG_LINE_SIZE];
     uint8_t index;
     uint16_t line_num;
     BOOL find;
     char buff[90];
     FUNC_RETVAL retval;

     retval = FUNC_ERROR;

     sprintf(buff,"cerco badge:%s  in ANAG.DAT",badge);
     debug_print_timestamp_ident(anag_work.dbg_ena,anag_work.dbg_idn,buff);
     
     // aprin il file in lettura
     if (anag_SD_openrd())
       {
          index = 0;
          line_num = 0;
          find = FALSE;
     
          *esito_ricerca = ID_EVENT_BADGE_SCONOSCIUTO;
          while (anag_char_available() && (!find))
            {
               c = anag_read_char();
               switch (c)
                 {
                    case 0x0A:
                      index = 0;
                      line_num ++;
                 
                      sprintf(buff,"L%04d  %s",line_num,line);
                      debug_print_timestamp_ident(anag_work.dbg_ena,anag_work.dbg_idn,buff);
                 
                      if (strstr(line,badge))
                        {
                           *esito_ricerca = ID_EVENT_BADGE_ABILITATO;
                           find = TRUE;
                        }
                      break;
                 
                    case 0x0D:
                      break;
                 
                    default:
                      if (index < ANAG_LINE_SIZE-1)
                        {
                           line[index] = c;
                           index++;
                        }
                 }              
            }       
          anag_SD_close();

          debug_print_timestamp_ident(anag_work.dbg_ena,anag_work.dbg_idn,(char *)"fine ricerca : ");
          if (find)
            debug_print_ena(anag_work.dbg_ena,(char *)"badge trovato ..");
          else
            debug_print_ena(anag_work.dbg_ena,(char *)"badge sconosciuto ..");

          retval = FUNC_OK;
       }
             
     return retval;
  } 
