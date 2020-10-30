/****************************************************************************/
/* File:           LK_mapping.cpp                                           */
/* Data creazione: 30/10/2020                                               */
/* Creato da:      Ivan De Stefani                                          */
/* Funzione:       Modulo per la gestione della mappatura degli sportelli   */
/*                 dell'armadio Argo LK Pro tramite scaricamento di un      */
/*                 file descrittivo dal server.                             */
/* Changelog:                                                               */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#include "Arduino.h"
#include "stdint.h"
#include "stdio.h"
#include "GP_Library/GP_library.h"
#include "Hardware/Debug/debug_HW.h"
#include "LK_mapping.h"

MAP_DATA LK_map_data;

const char str_mapping_init_start[] PROGMEM  = { "|-- Inizializzo la mappatura degli sportelli da server --|" };
const char str_mapping_init_end[] PROGMEM    = { "|-- Fine inizializzazione della mappatura degli sportelli da server. --|" };
const char st_mapping_init[] PROGMEM         = { "ST_MAPPING_INIT" };
const char st_mapping_check_lan[] PROGMEM    = { "ST_MAPPING_CHECK_LAN" };
const char st_mapping_get_from_lan[] PROGMEM = { "ST_MAPPING_GET_FROM_LAN" };
const char st_mapping_get_from_sd[] PROGMEM  = { "ST_MAPPING_GET_FROM_SD" };
const char st_mapping_done[] PROGMEM         = { "ST_MAPPING_DONE" };
const char st_mapping_error[] PROGMEM        = { "ST_MAPPING_ERROR" };
const char st_mapping_fail[] PROGMEM         = { "ST_MAPPING_FAIL" };

const char* const mapping_state_name[] PROGMEM = {
  st_mapping_init,
  st_mapping_check_lan,
  st_mapping_get_from_lan,
  st_mapping_get_from_sd,
  st_mapping_done,
  st_mapping_error,
  st_mapping_fail
};

FSM_WORK FSM_Mapping;

void mapping_init(void)
{
  debug_message_enable(TRUE,2);
  debug_message_CRLF();
  debug_message_timestamp_PGM(str_mapping_init_start);
  
  fsm_init(&FSM_Mapping,
            ST_MAPPING_INIT,
            (void *)mapping_state_name,
            ST_MAPPING_FAIL,
            (char *)"[MAP]");

  debug_message_timestamp_PGM(str_mapping_init_end);
}

void mapping_hand(void)
{
  switch (FSM_Mapping.state)
  {
      case ST_MAPPING_INIT:
        break;
      
      case ST_MAPPING_CHECK_LAN:
        break;
      
      case ST_MAPPING_GET_FROM_LAN:
        break;
      
      case ST_MAPPING_GET_FROM_SD:
        break;
      
      case ST_MAPPING_DONE:
        break;
      
      case ST_MAPPING_ERROR:
        break;
      
      case ST_MAPPING_FAIL:
        break;
      
      default:
        break;
  }
}