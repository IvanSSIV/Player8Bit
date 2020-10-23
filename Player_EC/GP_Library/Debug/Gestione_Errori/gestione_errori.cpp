#include "player_EC.h"
// Stringhe per invio errori al server

char* error_msg[] =
{
	"ERRORE_PROVA",				// Indice zero vuoto per allineamento con definizioni mnemoniche
	"ERRORE_ZERO",
	"ERRORE_DENTI",
	"ERRORE_LOGICA",
	"ERRORE_PLC",
	"ERRORE_SD",
	"ERRORE_EEPROM",
	"ERRORE_SERVER",
	"ERRORE_RTC"
};

/******************************************/
/* function send_error_report()           */
/* invia al server il report di un errore */
/******************************************/
void send_error_report(uint8_t err_code)
{
	//Server_send_event(SRV_EV_SEND_MSG, error_msg[err_code]);
}