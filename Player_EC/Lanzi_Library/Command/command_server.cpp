#include "..\Lanzi_library.h"
#include "command_server.h"

static const char str_server_cmd_accesa[] PROGMEM     = { "invio messaggio ACCESA" };
static const char str_server_cmd_aperta[] PROGMEM     = { "invio messaggio APERTA" };
static const char str_server_cmd_chiusa[] PROGMEM     = { "invio messaggio CHIUSA" };
static const char str_server_cmd_pctime[] PROGMEM     = { "invio messaggio PCTIME" };
static const char str_server_cmd_OK[] PROGMEM         = { "OK" };

static void command_accesa(void);
static void command_aperta(void);
static void command_chiusa(void);
static void command_pctime(void);

BOOL serial_ESC_LANZI_server(char c) 
  {
     //debug_message_timestamp("ESC_LANZI server");
     switch (c)
       {
          case '1':
            command_accesa();
            break;
          case '2':
            command_aperta();
            break;
          case '3':
            command_chiusa();
            break;
          case '4':
            command_pctime();
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



static void command_accesa(void) 
  {
     debug_message_timestamp_PGM(str_server_cmd_accesa);
     if (server_request_send(SRV_MSGID_ACCESA,""))
       {
          debug_message_timestamp_PGM(str_server_cmd_OK);
       }
  }

static void command_aperta(void) 
  {
     debug_message_timestamp_PGM(str_server_cmd_aperta);
     if (server_request_send(SRV_MSGID_APERTA,""))
       {
          debug_message_timestamp_PGM(str_server_cmd_OK);
       }
  }

static void command_chiusa(void) 
  {
     debug_message_timestamp_PGM(str_server_cmd_chiusa);
     if (server_request_send(SRV_MSGID_CHIUSA,""))
       {
          debug_message_timestamp_PGM(str_server_cmd_OK);
       }
  }

static void command_pctime(void) 
  {
     debug_message_timestamp_PGM(str_server_cmd_pctime);
     if (server_request_send(SRV_MSGID_PCTIME,""))
       {
          debug_message_timestamp_PGM(str_server_cmd_OK);
       }
  }

