#include "player_EC.h"
#if MACHINE_TYPE == ARGO_EC
const char str_command_erog_start[] PROGMEM = { "[ESC_CMD] start procedura erogazione tappi" };
const char str_command_erog_stop[] PROGMEM  = { "[ESC_CMD] stop  procedura erogazione tappi" };

// comandi ESC specifici per player EC
// ESC+PEC1 = avvia erogazione
// ESC+PEC2 = ferma erogazione

BOOL serial_ESC_player(char c)
  {
     char buff[80];
     sprintf(buff,"ricevuto comando <ESC>PEC%c",c);
     debug_message_timestamp(buff);

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