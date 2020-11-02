
#include <Ethernet.h>
#include <utility/w5100.h>
// Enumerazione per stati di gestione scheda SD

enum SD_state 
{
	SD_ST_INIT = 0,
	SD_ST_READ_CONFIG,
	SD_ST_WRITE_CONFIG,
	SD_ST_READ_ANAG,
	SD_ST_WRITE_ANAG,
	SD_ST_READ_FILLMAP,
	SD_ST_WRITE_FILLMAP,
	SD_ST_READ_AUTH,
	SD_ST_WRITE_AUTH,
	SD_ST_WRITE_REFILL,
	SD_ST_READ_REFILL,
	SD_ST_WRITE_MV,
	SD_ST_READ_OFFLINE,
	SD_ST_WRITE_OFFLINE
};

// Nomi costanti dei file gestiti da SD

enum SD_files {
	FILE_CONFIG = 0,
	FILE_ANAG,
	FILE_APERTURA,
	FILE_CHIUSURA,
	FILE_RICARICA
};

// Struttura per salvataggio dati macchina
struct VM_data 
{
	char NrComputer[7];
	char TipoMacchina[7];
	IPAddress IPMacchina;
	IPAddress Server;
	IPAddress Gateway;
	IPAddress SubnetMask;
	IPAddress DNS;
	uint8_t MAC;
	uint32_t port;
};

typedef struct VM_data VM_DATA;


// prototipi di funzione
void SD_init(void);			// Inizializzazione scheda SD con failure check
void SD_append(uint8_t sdfileindex, char* sdbuff); // Aggiunge a un file, se già esistente
BOOL SD_create(uint8_t sdfileindex, char* sdbuff); // Crea un file, se non esiste
BOOL SD_read(uint8_t sdfileindex, char* sdbuff, uint32_t start, uint32_t end); // Legge in un file da start a end
BOOL SD_read_config(VM_DATA* vm);