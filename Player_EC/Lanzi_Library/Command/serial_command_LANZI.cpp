#include "..\Lanzi_library.h"
//#include "command_server.h"
// prototipi funzione locali
static BOOL serial_ESC_LANZI_anagrafica(char c,uint8_t *stat);
static BOOL serial_ESC_LANZI_config(char c,uint8_t *stat);

/*- L = chiamata funzioni di libreria LANZI
    - <ESC>LC comandi Config
    - <ESC>LA comandi Anagrafica
    - <ESC>LB comandi Badge
    - <ESC>LS comandi Server
    - <ESC>LO comandi Offline
    - <ESC>LV comandi Virtual_server
    - <ESC>LT comandi Transazioni */

/*------------------------------------------------------------*/
/* function serial_ESC_LANZI()                                */
/* Gestisce una sequenza <ESC>Lx relativa alle librerie LANZI */
/* In uscita restituisce FALSE se il comando deve proseguire  */
/* oppure TRUE se concluso o errore                           */
/*------------------------------------------------------------*/
BOOL serial_ESC_LANZI(char c,ESC_HAND_STATE *stat)
  {
     uint8_t ESC_stat;
     BOOL retval;
     static char ESC_L_selection;

     ESC_stat = *stat;

     //char buff[80];
     //sprintf(buff,"ESC_LANZI st=%02x chr:%c sel=%c",
     //        ESC_stat,c,ESC_L_selection);
     //debug_message_timestamp(buff);

     retval = FALSE;
     switch (ESC_stat)
       {
          case ST_ESC_CH2:
            //debug_message_timestamp("comando ESC_Lx stato = 2");
            switch (c)
              {
                 case 'C':
                 case 'A':
                 case 'B':
                 case 'S':
                 case 'O':
                 case 'V':
                 case 'T':
                   *stat = ST_ESC_CH3;
                   ESC_L_selection = c;
                   break;
                 default:
                   retval = TRUE;
              }
            break;

          case ST_ESC_CH3:
            //debug_message_timestamp("comando ESC_Lx stato = 3");
            switch (ESC_L_selection)
              {
                 case 'C':
                   retval = serial_ESC_LANZI_config(c);
                   break;
                 case 'A':
                   retval = serial_ESC_LANZI_anagrafica(c);
                   break;
                 case 'B':
                   retval = serial_ESC_LANZI_badge(c);
                   break;
                 case 'S':
                   retval = serial_ESC_LANZI_server(c);
                   break;
                 case 'O':
                   debug_message_timestamp("comando ESC_LC (offline)");
                   break;
                 case 'V':
                   debug_message_timestamp("comando ESC_LC (Virtual_server)");
                   break;
                 case 'T':
                   debug_message_timestamp("comando ESC_LC (config)");
                   break;
                 default:
                   retval = TRUE;
              }
            break;

       }
     return retval;
  }




