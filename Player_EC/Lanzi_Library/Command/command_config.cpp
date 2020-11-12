#include "..\Lanzi_library.h"
#include "command_config.h"

static const char str_config_read_server[] PROGMEM = { "# richiesta lettura config da server #" };
static const char str_server_cmd_OK[] PROGMEM      = { "OK" };

// prototipi funzione locali
static void config_read_from_server(void);
static void config_read_from_SD(void);

BOOL serial_ESC_LANZI_config(char c) 
  {
     switch (c)
       {
          case '1':
            config_read_from_server();
            break;
          case '2':
            config_read_from_SD();
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

static void config_read_from_server(void)
  {
     debug_message_timestamp_PGM(str_config_read_server);
     if (server_request_send(SRV_MSGID_RICHIESTA_CONFIG,""))
       {
          debug_message_timestamp_PGM(str_server_cmd_OK);
       }
  }

static void config_read_from_SD(void)
  {

  }
