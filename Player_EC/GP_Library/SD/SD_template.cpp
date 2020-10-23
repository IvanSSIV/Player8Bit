/****************************************************************************/
/* File:           SD_template.cpp                                          */
/* Data creazione: 27/03/2020 												*/
/* Creato da:      Ivan De Stefani											*/
/* Funzione:       Modulo di gestione della scheda microSD.        			*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/
// include comuni in libreria "GP_library"
#include "../GP_library.h"
#include "SD.h"

// Stringhe costanti di debug su seriale
const char str_dbg_SD_init_start[] PROGMEM = { "Inizializzo scheda microSD : " };
const char str_dbg_SD_init_OK[] PROGMEM    = { "OK" };
const char str_dbg_SD_init_KO[] PROGMEM    = { "FAIL: Card not working, or not present." };
const char str_dbg_SD_init_end[] PROGMEM   = { "Fine inizializzazione scheda microSD." };

// Dichiarazioni locali
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;
File data_file;			// Variabile di accesso ai file presenti sulla scheda microSD
BOOL SD_failure;		// Flag di fallimento inizializzazione scheda microSD

// definizione per trasformazione caratteri letti da ascii a char
uint16_t ascii_char[36] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
						   77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90 };

// vettore dei nomi costanti assegnati ai file sulla scheda microSD
const char str_file_CONFIG[] PROGMEM   = { "CONFIG.DAT" };
const char str_file_ANAG[] PROGMEM     = { "ANAG.DAT" };
const char str_file_APERTURA[] PROGMEM = { "APERTURA.DAT" };
const char str_file_CHIUSURA[] PROGMEM = { "CHIUSURA.DAT" };
const char str_file_RICARICA[] PROGMEM = { "RICARICA.DAT" };

const char* const SD_file_name[] PROGMEM = {
	str_file_CONFIG,
	str_file_ANAG,
	str_file_APERTURA,
	str_file_CHIUSURA,
	str_file_RICARICA
};

/**************************************************/
/* void SD_init(void)                             */
/*________________________________________________*/
/* Inizializza la comunicazione con la scheda SD. */
/*                                                */
/* Parametri: nessuno                             */
/**************************************************/

void SD_init(void)
  {
	SD_failure = FALSE;
	debug_print_timestamp(sd_dbg, AVR_PGM_to_str(str_dbg_SD_init_start));
	if (SD.begin()) 
	  {
		debug_print_ena(sd_dbg, AVR_PGM_to_str(str_dbg_SD_init_OK));
	  }
	else
	  {
		SD_failure = TRUE;
		debug_print_ena(sd_dbg, AVR_PGM_to_str(str_dbg_SD_init_KO));
	  }

	debug_print_timestamp(sd_dbg, AVR_PGM_to_str(str_dbg_SD_init_end));
  }

/**************************************************************************/
/* void SD_append(uint8_t sdfileindex, char * sdbuff)					  */
/*________________________________________________________________________*/
/* Aggiunge al file identificato dal numero sdfileindex                   */
/* la stringa sdbuff.									                  */
/*																	      */
/* Parametri: sdfileindex: indice dell'array che contiene i nomi dei file */
/*            sdbuff: stringa da aggiungere al file                       */
/**************************************************************************/
void SD_append(uint8_t sdfileindex, char* sdbuff)
  {
	char answbuff[50];
	if (SD.exists(AVR_PGM_to_str(SD_file_name[sdfileindex]))) 
	  {
		 sprintf(answbuff, "Esiste il file %s", AVR_PGM_to_str(SD_file_name[sdfileindex]));
		 debug_print_timestamp(sd_dbg, (char*)answbuff);
		 data_file = SD.open(AVR_PGM_to_str(SD_file_name[sdfileindex]), FILE_WRITE);
		 data_file.println(sdbuff);
		 data_file.close();
	  }
	else
	{
		sprintf(answbuff, "Non esiste il file %s", AVR_PGM_to_str(SD_file_name[sdfileindex]));
		debug_print_timestamp(sd_dbg, (char*)answbuff);
	}
  }

/**************************************************************************/
/* BOOL SD_create(uint8_t sdfileindex, char * sdbuff)					  */
/*________________________________________________________________________*/
/* Se non esiste, crea sulla scheda SD il file indicato dal numero        */
/* sdfileindex nell'array contenente i nomi dei file.  Restituisce TRUE   */ 
/* se il file non esiste, FALSE se esiste già.             				  */	
/*																	      */
/* Parametri: sdfileindex: indice dell'array che contiene i nomi dei file */
/*            sdbuff: stringa da aggiungere al file                       */
/**************************************************************************/
BOOL SD_create(uint8_t sdfileindex, char* sdbuff)
  {
	BOOL ret_val_create = FALSE;
	if (!SD.exists(AVR_PGM_to_str(SD_file_name[sdfileindex])))
	  {
		ret_val_create = TRUE;
		data_file = SD.open(AVR_PGM_to_str(SD_file_name[sdfileindex]), FILE_WRITE);
		if (sizeof(sdbuff) > 0)
		  {
		   data_file.println(sdbuff);
		  }
		data_file.close();
	  }
	return ret_val_create;
  }

/********************************************************************************/
/* BOOL SD_read(uint8_t sdfileindex, char sdbuff, uint32_t start, uint32_t end) */
/*______________________________________________________________________________*/
/* Legge dal file indicato da sdfileindex i caratteri dall'indirizzo start      */
/* all'indirizzo end inclusi, e li salva in sdbuff. Restituisce TRUE se il file */
/* esiste, FALSE se non esiste.													*/		
/*																			    */
/* Parametri: sdfileindex: indice dell'array che contiene i nomi dei file		*/
/*            sdbuff: stringa da aggiungere al file								*/
/*            start: indice da cui iniziare la lettura							*/
/*            end: indice a cui terminare la lettura							*/
/********************************************************************************/
BOOL SD_read(uint8_t sdfileindex, char * sdbuff, uint32_t start, uint32_t end)
  {
	BOOL ret_val_read = FALSE;
	uint16_t tmpbuf[80];
	if (SD.exists(AVR_PGM_to_str(SD_file_name[sdfileindex])))
	  {
		data_file = SD.open(AVR_PGM_to_str(SD_file_name[sdfileindex]), FILE_READ);
			ret_val_read = TRUE;
			data_file.seek(start);

		for (uint32_t i = start; i <= end; i++)
		{
			tmpbuf[i] = data_file.read();
			strncat(sdbuff, (char*)tmpbuf[i], sizeof(sdbuff) - 1);
		}
	  }
	return ret_val_read;
  }

/********************************************************************************/
/* BOOL SD_read_config(VM_DATA * vm)											*/
/*______________________________________________________________________________*/
/* Legge il file config e salva i valori nei parametri della struttura VM_DATA. */
/*                                                                              */
/* Parametri: VM_DATA * vm: Puntatore alla struttura da inizializzare			*/
/********************************************************************************/

// Definizione indici costanti per lettura config
#define NRPC_START					 0
#define NRPC_END					 4
#define TIPOPC_START				 7
#define TIPOPC_END					12
#define IP_START					15
#define IP_END						29
#define SM_START					32
#define SM_END						46
#define GW_START					49
#define GW_END						63
#define DNS_START					66
#define DNS_END						80
#define MAC_START					83
#define MAC_END						88
#define SRV_START					91
#define SRV_END					   105
#define PORT_START				   108
#define PORT_END				   112
#define SECNUM_START			   115
#define SECNUM_END				   117

// Stringhe per stampa di debug del config
const char str_config_start_read[] PROGMEM = { "Inizio lettura config ..." };
const char str_config_num_comp[] PROGMEM = { "Numero computer: " };
const char str_config_tipo_comp[] PROGMEM = { "Tipo computer: " };
const char str_config_ip_macch[] PROGMEM = { "IP macchina: " };
const char str_config_sm[] PROGMEM = { "Subnet Mask: " };
const char str_config_gw[] PROGMEM = { "Gateway: " };

BOOL SD_read_config(VM_DATA* vm)
  {
	 static uint16_t index;
	 static uint8_t vecbuff;
	 char buff[20];
	 BOOL ret_val_config = FALSE;
	 if (SD.exists(AVR_PGM_to_str(SD_file_name[FILE_CONFIG])))
	   {

	#ifdef DEBUG_VERSION
		debug_print_timestamp(sd_dbg, AVR_PGM_to_str(str_config_start_read));
	#endif
				 
		  ret_val_config = TRUE;
		  vecbuff = 0;
		  memset(buff, 0, sizeof(buff));
		  data_file = SD.open(AVR_PGM_to_str(SD_file_name[FILE_CONFIG]), FILE_READ);
		  debug_print_timestamp_ident(sd_dbg, 3, AVR_PGM_to_str(str_config_num_comp));
		  for (index = NRPC_START; index <= NRPC_END; index++)
		    {
			  vm->NrComputer[vecbuff] = data_file.read();
			  debug_print_ena(sd_dbg,(char*)vm->NrComputer[vecbuff]);
			  index++;
			  vecbuff++;
		    }
		  vecbuff = 0;
		  debug_print_timestamp_ident(sd_dbg, 3, AVR_PGM_to_str(str_config_tipo_comp));

		  for (index = TIPOPC_START; index <= TIPOPC_END; index++)
		  {
			  vm->TipoMacchina[vecbuff] = data_file.read();
			  debug_print_ena(sd_dbg, (char*)vm->NrComputer[vecbuff]);
			  index++;
			  vecbuff++;
		  }
		  vecbuff = 0;
		  
		  data_file.read();
		  data_file.read();
		  
		  // Lettura IP
		  debug_print_timestamp_ident(sd_dbg, 3, AVR_PGM_to_str(str_config_ip_macch));
		  vm->IPMacchina[0] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->IPMacchina[1] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->IPMacchina[2] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->IPMacchina[3] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48);
		  data_file.read();
		  data_file.read();
		  debug_print_ena(sd_dbg, (char*)vm->IPMacchina[0]);
		  debug_print_ena(sd_dbg, (char*)vm->IPMacchina[1]);
		  debug_print_ena(sd_dbg, (char*)vm->IPMacchina[2]);
		  debug_print_ena(sd_dbg, (char*)vm->IPMacchina[3]);

		  // Maschera di sottorete
		  debug_print_timestamp_ident(sd_dbg, 3, AVR_PGM_to_str(str_config_sm));
		  vm->SubnetMask[0] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->SubnetMask[1] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->SubnetMask[2] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->SubnetMask[3] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48);
		  data_file.read();
		  data_file.read();
		  debug_print_ena(sd_dbg, (char*)vm->SubnetMask[0]);
		  debug_print_ena(sd_dbg, (char*)vm->SubnetMask[1]);
		  debug_print_ena(sd_dbg, (char*)vm->SubnetMask[2]);
		  debug_print_ena(sd_dbg, (char*)vm->SubnetMask[3]);

		  // Gateway
		  debug_print_timestamp_ident(sd_dbg, 3, AVR_PGM_to_str(str_config_gw));
		  vm->Gateway[0] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->Gateway[1] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->Gateway[2] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48); data_file.read();
		  vm->Gateway[3] = 100 * (data_file.read() - 48) + 10 * (data_file.read() - 48) + (data_file.read() - 48);
		  data_file.read();
		  data_file.read();
		  debug_print_ena(sd_dbg, (char*)vm->Gateway[0]);
		  debug_print_ena(sd_dbg, (char*)vm->Gateway[1]);
		  debug_print_ena(sd_dbg, (char*)vm->Gateway[2]);
		  debug_print_ena(sd_dbg, (char*)vm->Gateway[3]);
	   }
  } 
