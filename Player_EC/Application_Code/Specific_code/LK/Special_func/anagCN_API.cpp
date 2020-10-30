/****************************************************************************/
/* File:           anagCN_API.cpp                                           */
/* Data creazione: 30/10/2020                                               */
/* Creato da:      Ivan De Stefani                                          */
/* Funzione:       Modulo per la gestione degli stati di erogazione dell'   */
/*                 armadio Argo LK Pro.                                     */
/* Changelog:                                                               */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#include "Arduino.h"
#include "stdint.h"
#include "stdio.h"
#include "GP_Library/GP_library.h"
#include "Hardware/Debug/debug_HW.h"
#include "anagCN_API.h"

FSM_WORK FSM_anagCN;

static const char str_st_anagcn_init[] PROGMEM           = {"ST_ANAGCN_INIT"};
static const char str_st_anagcn_check_lan[] PROGMEM      = {"ST_ANAGCN_CHECK_LAN"};
static const char str_st_anagcn_delete_file[] PROGMEM    = {"ST_ANAGCN_DELETE_FILE"};
static const char str_st_anagcn_send_request[] PROGMEM   = {"ST_ANAGCN_SEND_REQUEST"};
static const char str_st_anagcn_read_line[] PROGMEM      = {"ST_ANAGCN_READ_LINE"};
static const char str_st_anagcn_decode_line[] PROGMEM    = {"ST_ANAGCN_DECODE_LINE"};
static const char str_st_anagcn_read_last_line[] PROGMEM = {"ST_ANAGCN_READ_LAST_LINE"};
static const char str_st_anagcn_done[] PROGMEM           = {"ST_ANAGCN_DONE"};
static const char str_st_anagcn_error[] PROGMEM          = {"ST_ANAGCN_ERROR"};
static const char str_st_anagcn_fail[] PROGMEM           = {"ST_ANAGCN_FAIL"};

const char* const str_st_anagcn_names[] PROGMEM = {
  str_st_anagcn_init,
  str_st_anagcn_check_lan,
  str_st_anagcn_delete_file,
  str_st_anagcn_send_request,
  str_st_anagcn_read_line,
  str_st_anagcn_decode_line,
  str_st_anagcn_read_last_line,
  str_st_anagcn_done,
  str_st_anagcn_error,
  str_st_anagcn_fail
};

// Inizializzazione della funzione per lo scaricamento del file di anagrafica nomi e cognomi
void anagCN_init(void)
{
  fsm_init(&FSM_anagCN,
           ST_ANAGCN_INIT,
           (void *)str_st_anagcn_names,
           ST_ANAGCN_FAIL,
           (char *)"[ACN]");
}

// Scaricamento del file anagrafica nomi e cognomi
void FSM_get_anagCN(void)
{
  switch (FSM_anagCN.state)
  {
      case ST_ANAGCN_INIT:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_init)))
        {

        }
      break;
     
      case ST_ANAGCN_CHECK_LAN:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_check_lan)))
        {

        }
      break;
      
      case ST_ANAGCN_DELETE_FILE:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_delete_file)))
        {

        }
      break;
      
      case ST_ANAGCN_SEND_REQUEST:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_send_request)))
        {

        }
      break;
      
      case ST_ANAGCN_READ_LINE:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_read_line)))
        {

        }
      break;
      
      case ST_ANAGCN_DECODE_LINE:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_decode_line)))
        {

        }
      break;
      
      case ST_ANAGCN_READ_LAST_LINE:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_read_last_line)))
        {

        }
      break;
      
      case ST_ANAGCN_DONE:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_done)))
        {

        }
      break;
      
      case ST_ANAGCN_ERROR:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_error)))
        {

        }
      break;
      
      case ST_ANAGCN_FAIL:
        if (fsm_first_scan(&FSM_anagCN, AVR_PGM_to_str(str_st_anagcn_fail)))
        {

        }
      break;
      
      default:
      break;
  }
}

                                              /*****************************/
                                              /* FUNZIONI LOCALI AL MODULO */
                                              /*****************************/

USER_DATA anagcn_data;

static void get_user_data(char* user_string)
{
  char* ptr;
  uint8_t token_index;
  for (token_index = 0; token_index < 3; token_index++)
  {
    ptr = strtok(user_string, " ");
    switch (token_index)
    {
      case 0:
        strncpy(ptr, anagcn_data.badge, sizeof(anagcn_data.badge) - 1);
        break;
      case 1:
        strncpy(ptr, anagcn_data.nome, sizeof(anagcn_data.nome) - 1);
        break;
      case 2:
        strncpy(ptr, anagcn_data.cognome, sizeof(anagcn_data.cognome) - 1);
        break;
      default:
        break;
    }
  }

}


