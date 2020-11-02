#include "..\Lanzi_library.h"
#include "command_server.h"
// prototipi funzione locali
static BOOL serial_ESC_LANZI_anagrafica(char c,uint8_t *stat);
static BOOL serial_ESC_LANZI_config(char c,uint8_t *stat);

BOOL serial_ESC_LANZI(char c,ESC_HAND_STATE *stat)
  {
     //char buff[80];
     //sprintf(buff,"ricevuto comando <ESC>L%c",c);
     //debug_print_timestamp(TRUE,buff);
     switch (c)
       {
          case '1':
            break;
          case '2':
            break;
          case '3':
            break;
          case '4':
            break;
          case '5':
            break;

       }
     return TRUE;
  }

static BOOL serial_ESC_LANZI_anagrafica(char c,uint8_t *stat) 
  {
     return TRUE;
  }

static BOOL serial_ESC_LANZI_config(char c,uint8_t *stat) 
  {
     return TRUE;
  }
