#include "player_EC.h"

BOOL serial_ESC_test(char c)
  {
     char buff[80];
     sprintf(buff,"ricevuto comando <ESC>T%c",c);
     debug_print_timestamp(TRUE,buff);

     
     return TRUE;
  }