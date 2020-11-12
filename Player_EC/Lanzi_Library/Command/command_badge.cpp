#include "..\Lanzi_library.h"
#include "command_badge.h"
static const char str_badge_verify_online[] PROGMEM = { "# verifico badge su server #" };

static void badge_verify_online(void);

BOOL serial_ESC_LANZI_badge(char c) 
  {
     switch (c)
       {
          case '1':
            badge_verify_online();
            break;
          case '2':
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


void badge_verify_online(void)
  {
     debug_message_timestamp_PGM(str_badge_verify_online);
     if (server_request_send(SRV_MSGID_BADGE,"101957"))
       {
          //debug_message_timestamp_PGM(str_server_cmd_OK);
       }

  }
