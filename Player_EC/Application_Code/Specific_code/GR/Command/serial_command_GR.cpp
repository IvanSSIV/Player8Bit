#include "player_EC.h"

#if MACHINE_TYPE == ARGO_GR

const char str_command_erog_start[] PROGMEM = { "[ESC_CMD] start procedura erogazione" };
const char str_command_erog_stop[] PROGMEM  = { "[ESC_CMD] stop  procedura erogazione" };

BOOL serial_ESC_player(char c)
  {
     /*char buff[80];
     sprintf(buff,"ricevuto comando <ESC>P%c",c);
     debug_print_timestamp(TRUE,buff);*/

     switch (c)
       {
          case '0':
            break;
     
          case '1':
            if (erog_is_ready())
              {
                 debug_message_timestamp_PGM(str_command_erog_start);
                 erog_start();
              }
            break;
          
          case '2':
            erog_stop();
            debug_message_timestamp_PGM(str_command_erog_stop);
            break;
          
          case '3':
            break;
          case '4':
            break;
          case '5':
            break;
          case '6':
            break;
          case '7':
            break;
          case '8':
            break;
          case '9':
            break;
       }

     return TRUE;
  }

#endif