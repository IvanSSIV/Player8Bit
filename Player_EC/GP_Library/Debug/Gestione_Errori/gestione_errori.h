// Definizioni mnemoniche per invio errori al server
#define ERR_ZERO		1		// Mancato rilevamento del sensore di zero
#define ERR_MOTORE		2		// Mancato rilevamento del sensore della trasmissione
#define ERR_LOGICA		3		// Errore delle procedure logiche
#define ERR_PLC			4		// Errore dei GPIO
#define ERR_SD			5		// Errore di scheda SD
#define ERR_EEPROM		6		// Errore della memoria EEPROM
#define ERR_SERVER		7		// Errore di comunicazione con il server
#define ERR_RTC			8		// Errore del modulo RTC

void send_error_report(uint8_t err_code);