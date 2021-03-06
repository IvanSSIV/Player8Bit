/****************************************************************************/
/* File: EEPROM_template.cpp                                                */
/* Data creazione: 08/04/2020  												*/
/* Creato da: Ivan De Stefani 												*/
/* Funzione: Driver a basso livello della memoria EEPROM           			*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

#include "Arduino.h"
#include <Wire.h>
#include <EEPROM.h>

#include "debug_opzion.h"
#include "project_def.h"
#include "My_Include/mydef.h"

#include <My_Debug/Serial_Command/serial_command.h>
#include <My_Debug/Debug_Serial/debug_serial.h>
#include <My_Debug/Debug_Utility/debug_utility.h>
#include <My_Library/Utilities/AVR_API.h>
#include "EEPROM_template.h"


// Stringhe costanti per stampa messaggi su seriale di debug
const char str_dbg_EEP_init_start[] PROGMEM  = { "Inizializzo memoria EEPROM ..." };
const char str_dbg_EEP_init_OK[] PROGMEM     = { "EEPROM inizializzata correttamente!" };
const char str_dbg_EEP_init_KO[] PROGMEM     = { "EEPROM failure!" };
const char str_dbg_EEP_init_end[] PROGMEM    = { "Fine inizializzazione memoria EEPROM ..." };
const char str_dbg_EEP_saved_value[] PROGMEM = { "Salvato il valore " };
const char str_dbg_EEP_at_address[] PROGMEM  = { " all'indirizzo " };

static char eep_buff[80];

/***************************************/
/* function EEPROM_Init()              */
/* Inizializza la memoria EEPROM.      */
/***************************************/
void EEPROM_Init(void)
{
	debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_EEP_init_start));

	if (eeprom_is_ready())
	{
		debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_EEP_init_OK));
	}
	else
	{
		debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_EEP_init_KO));
	}

	debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_EEP_init_end));
}

/***************************************/
/* function EEPROM_save_8bit()         */
/* Salva un byte all'indirizzo dato    */
/***************************************/
void EEPROM_save_8bit(uint8_t *address, uint8_t value)
{
	char tmpbuff[20];
	strncpy(eep_buff, AVR_PGM_to_str(str_dbg_EEP_saved_value), sizeof(eep_buff) - 1);
	sprintf(tmpbuff, "%d", value);
	strncat(eep_buff, tmpbuff, sizeof(eep_buff) - 1);
	strncat(eep_buff, AVR_PGM_to_str(str_dbg_EEP_at_address), sizeof(eep_buff) - 1);
	memset(tmpbuff, 0, sizeof(tmpbuff));
	sprintf(tmpbuff, "%d", (uint8_t*)address);
	strncat(eep_buff, tmpbuff, sizeof(eep_buff) - 1);

	eeprom_write_byte((uint8_t *)address, value);

	debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L2, eep_buff);
}


/**********************************************/
/* function EEPROM_save_16bit()               */
/* Salva una word (16 bit) all'indirizzo dato */
/**********************************************/
void EEPROM_save_16bit(uint16_t* address, uint16_t value)
{
	char tmpbuff[20];
	strncpy(eep_buff, AVR_PGM_to_str(str_dbg_EEP_saved_value), sizeof(eep_buff) - 1);
	sprintf(tmpbuff, "%d", value);
	strncat(eep_buff, tmpbuff, sizeof(eep_buff) - 1);
	strncat(eep_buff, AVR_PGM_to_str(str_dbg_EEP_at_address), sizeof(eep_buff) - 1);
	memset(tmpbuff, 0, sizeof(tmpbuff));
	sprintf(tmpbuff, "%d", (uint16_t*)address);
	strncat(eep_buff, tmpbuff, sizeof(eep_buff) - 1);

	eeprom_write_word((uint16_t*)address, value);
	debug_print_timestamp_ident(eep_dbg, DEBUG_IDENT_L2, eep_buff);
}

/*****************************************************/
/* function EEPROM_save_32bit()                      */
/* Salva una doppia word (32 bit) all'indirizzo dato */
/*****************************************************/
void EEPROM_save_32bit(uint32_t* address, uint32_t value)
{
	char tmpbuff[20];
	strncpy(eep_buff, AVR_PGM_to_str(str_dbg_EEP_saved_value), sizeof(eep_buff) - 1);
	sprintf(tmpbuff, "%d", value);
	strncat(eep_buff, tmpbuff, sizeof(eep_buff) - 1);
	strncat(eep_buff, AVR_PGM_to_str(str_dbg_EEP_at_address), sizeof(eep_buff) - 1);
	memset(tmpbuff, 0, sizeof(tmpbuff));
	sprintf(tmpbuff, "%d", (uint32_t*)address);
	strncat(eep_buff, tmpbuff, sizeof(eep_buff) - 1);

	eeprom_write_dword((uint32_t*)address, value);

	debug_print_timestamp_ident(eep_dbg, DEBUG_IDENT_L2, eep_buff);
}

/***************************************/
/* function EEPROM_read_8bit()         */
/* Legge un byte dall'indirizzo dato   */
/***************************************/
uint8_t EEPROM_read_8bit(uint8_t* address)
{
	return eeprom_read_byte((uint8_t*)address);
	debug_print_timestamp_ident(eep_dbg, DEBUG_IDENT_L2, eep_buff);
}


/***********************************************/
/* function EEPROM_read_16bit()                */
/* Legge una word (16 bit) dall'indirizzo dato */
/***********************************************/
uint16_t EEPROM_read_16bit(uint16_t* address)
{
	return eeprom_read_word((uint16_t*)address);
	debug_print_timestamp_ident(eep_dbg, DEBUG_IDENT_L2, eep_buff);
}

/******************************************************/
/* function EEPROM_read_32bit()                       */
/* Legge una doppia word (32 bit) dall'indirizzo dato */
/******************************************************/
uint32_t EEPROM_read_32bit(uint32_t* address)
{
	return eeprom_read_dword((uint32_t*)address);
	debug_print_timestamp_ident(eep_dbg, DEBUG_IDENT_L2, eep_buff);
}