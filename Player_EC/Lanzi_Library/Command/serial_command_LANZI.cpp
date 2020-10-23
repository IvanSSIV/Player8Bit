#include "..\Lanzi_library.h"

BOOL serial_ESC_LANZI(char c)
  {
     char buff[80];
     sprintf(buff,"ricevuto comando <ESC>L%c",c);
     debug_print_timestamp(TRUE,buff);
     return TRUE;
  }
