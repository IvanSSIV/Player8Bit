#define MODULE_APPL_LOGICA

/****************************************************************************/
/* File:           appl_logica                                              */
/* Data creazione: 15/05/2020 												*/
/* Creato da:      Ivan De Stefani   										*/
/* Funzione:       Modulo di gestione della logica principale dell'Argo 	*/
/*				   EC Pro.      											*/
/*																			*/
/****************************************************************************/

#include "player_header.h"

// Macchina a stati per la gestione della logica
FSM_WORK FSM_Logica;

// "Foto" delle principali variabili globali contenente le informazioni più importanti
// sullo stato della macchina
struct appl_logica_data
  {
	 BOOL flg_boot;  // primo passaggio dopo reset
	 BOOL flg_LAN_OK; // esito test connessione
	 BOOL flg_serv_config_OK;  // config letto dal server OK
	 BOOL flg_SD_config_OK;    // config su SD dal server OK
	 BOOL flg_serv_anag_OK;    // anagrafica letta da server OK
	 BOOL flg_SD_anag_OK;      // anagrafica su SD OK
	 char last_read_badge[MAX_BADGE_SIZE + 1]; // ultimo badge letto

  };
typedef struct appl_logica_data APPL_LOGIC_DATA;

static APPL_LOGIC_DATA appl_logic_data;

// Vettore con i nomi degli stati
char* state_name[] = {
	"ST_LOGIC_INIT",                              // Stato di inizializzazione
	"ST_LOGIC_MAIN",							  // Stato di attesa passaggio badge
	"ST_LOGIC_SEND_OFFLINE",					  // Richiesta PCTime e invio eventuali file offline
	"ST_LOGIC_BADGE",							  // Lettura del badge
	"ST_LOGIC_CHECK_LAN",						  // Verifica stato della connessione
	"ST_LOGIC_HAND_ONLINE",	     			      // Gestione autorizzazioni online
	"ST_LOGIC_HAND_OFFLINE",					  // Gestione autorizzazioni offline
	"ST_LOGIC_EROG",						      // Erogazione dei tappi
	"ST_LOGIC_NON_TROVATO", 					  // Badge non trovato nell'anagrafica online o offline
	"ST_LOGIC_RICHIESTA_AUTORIZZAZIONE",		  // Richiesta autorizzazione al prelievo
	"ST_LOGIC_ERROR",							  // Errore nella logica
	"ST_LOGIC_FAIL" 							  // Failure non recuperabile
};

// Stringhe in program memory per stampa di debug
const char str_log_st_init[] PROGMEM                     = { "ST_LOGIC_INIT" };
const char str_log_st_main[] PROGMEM                     = { "ST_LOGIC_MAIN" };
const char str_log_st_send_offline[] PROGMEM             = { "ST_LOGIC_SEND_OFFLINE" };
const char str_log_st_badge[] PROGMEM                    = { "ST_LOGIC_BADGE" };
const char str_log_st_check_lan[] PROGMEM                = { "ST_LOGIC_CHECK_LAN" };
const char str_log_st_hand_online[] PROGMEM              = { "ST_LOGIC_HAND_ONLINE" };
const char str_log_st_hand_offline[] PROGMEM             = { "ST_LOGIC_HAND_OFFLINE" };
const char str_log_st_erog[] PROGMEM                     = { "ST_LOGIC_EROG" };
const char str_log_st_non_trovato[] PROGMEM              = { "ST_LOGIC_NON_TROVATO" };
const char str_log_st_richiesta_autorizzazione[] PROGMEM = { "ST_LOGIC_RICHIESTA_AUTORIZZAZIONE" };
const char str_log_st_error[] PROGMEM                    = { "ST_LOGIC_ERROR" };
const char str_log_st_fail[] PROGMEM                     = { "ST_LOGIC_FAIL" };
const char str_log_init_start[] PROGMEM                  = { "Inizializzazione appl_logica..." };
const char str_log_init_end[] PROGMEM                    = { "Fine inizializzazione appl_logica!" };

// Prototipi di funzione locali
static void hand_ST_LOGIC_INIT(void);
static void hand_ST_LOGIC_MAIN(void);
static void hand_ST_LOGIC_SEND_OFFLINE(void);
static void hand_ST_LOGIC_BADGE(void);
static void hand_ST_LOGIC_CHECK_LAN(void);
static void hand_ST_LOGIC_HAND_ONLINE(void);
static void hand_ST_LOGIC_HAND_OFFLINE(void);
static void hand_ST_LOGIC_EROG(void);
static void hand_ST_LOGIC_NON_TROVATO(void);
static void hand_ST_LOGIC_RICHIESTA_AUTORIZZAZIONE(void);
static void hand_ST_LOGIC_ERROR(void);
static void hand_ST_LOGIC_FAIL(void);

/***************************************/
/* logic_init()                        */
/* Inizializzazione della macchina a   */
/* stati per la gestione della logica. */
/***************************************/
void logic_init(void)
{
	debug_print_timestamp(lgc_dbg, AVR_PGM_to_str(str_log_init_start));

	fsm_init(&FSM_Logica, 
		     ST_LOGIC_INIT,
		     state_name,
			 ST_LOGIC_FAIL,
		     "LGC");

	fsm_ena_print(&FSM_Logica, TRUE, TRUE, TRUE);

	appl_logic_data.flg_boot = TRUE; // segnala primo passaggio dopo reset
	appl_logic_data.flg_LAN_OK = FALSE;
	appl_logic_data.flg_SD_anag_OK =
	appl_logic_data.flg_SD_config_OK =
	appl_logic_data.flg_serv_config_OK =
	appl_logic_data.flg_serv_anag_OK = FALSE;
	
	debug_print_timestamp(lgc_dbg, AVR_PGM_to_str(str_log_init_end));
}

/***************************************/
/* logic_main()                        */
/* Gestione della logica principale.   */
/***************************************/
void logic_main(void)
{
	switch (FSM_Logica.state)
	  {
			case ST_LOGIC_INIT:
				hand_ST_LOGIC_INIT();
				break;
			case ST_LOGIC_MAIN:
				hand_ST_LOGIC_MAIN();
				break;
			case ST_LOGIC_SEND_OFFLINE:
				hand_ST_LOGIC_SEND_OFFLINE();
				break;
			case ST_LOGIC_BADGE:
				hand_ST_LOGIC_BADGE();
				break;
			case ST_LOGIC_CHECK_LAN:
				hand_ST_LOGIC_CHECK_LAN();
				break;
			case ST_LOGIC_HAND_ONLINE:
				hand_ST_LOGIC_HAND_ONLINE();
				break;
			case ST_LOGIC_HAND_OFFLINE:
				hand_ST_LOGIC_HAND_OFFLINE();
				break;
			case ST_LOGIC_EROG:
				hand_ST_LOGIC_EROG();
				break;
			case ST_LOGIC_NON_TROVATO:
				hand_ST_LOGIC_NON_TROVATO();
				break;
			case ST_LOGIC_RICHIESTA_AUTORIZZAZIONE:
				hand_ST_LOGIC_RICHIESTA_AUTORIZZAZIONE();
				break;
			case ST_LOGIC_ERROR:
				hand_ST_LOGIC_ERROR();
				break;
			case ST_LOGIC_FAIL:
				hand_ST_LOGIC_FAIL();
				break;
			default:
				;
	  }
}

////////////////////////// FUNZIONI DI GESTIONE DELLE AZIONI DELLA MACCHINA A STATI DELLA LOGICA /////////////////////////

static void hand_ST_LOGIC_INIT(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_init)))
	  {
		config_anag_init(); // Inizializza modulo per operazioni di scaricamento CONFIG e ANAG
		if (server_LAN_connection())
		{
			appl_logic_data.flg_LAN_OK = TRUE; 
		}
		else
		{
			appl_logic_data.flg_LAN_OK = FALSE;
			// Salto la richiesta del config, dell'anag e del PCTime e vado direttamente nel MAIN
			fsm_set_state(&FSM_Logica, ST_LOGIC_MAIN);
		}
		   
	  }
	else
	  {
		 // gestisce i dettagli delle operazioni di lettura del config e anagrafica
		 uint8_t esito = config_anag_hand();
		 switch (esito)
		   {
		      case ST_INIT_SUBSTATE_DONE: 
				  fsm_set_state(&FSM_Logica, ST_LOGIC_MAIN);
				  break;
			  case ST_INIT_SUBSTATE_ERROR:
				  fsm_set_state(&FSM_Logica, ST_LOGIC_ERROR);
				  break;
			  case ST_INIT_SUBSTATE_FAIL:
				  fsm_set_state(&FSM_Logica, ST_LOGIC_FAIL);
				  break;
		   }	   
	  }
}

#define PCTIME_REQUEST_RATE 60000L

static void hand_ST_LOGIC_MAIN(void)
  {
	static uint32_t PCtime_request_timer;

	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_main)))
	  {
		 // leggi dal modulo "logic_init" l'esito delle attivita di caricamento di config e anag
		 LOGIC_INIT_RETVAL tmp;
		 tmp = get_config_anag_status();
		 appl_logic_data.flg_serv_config_OK = tmp.flg_serv_config_OK;
		 appl_logic_data.flg_serv_anag_OK = tmp.flg_serv_anag_OK;
		 badge_ena_read();
		 PCtime_request_timer = RTOS_get_tick_1ms();
	  }
	else // Operazioni effettuate in "loop" nel main di applicazione
	  {
		 // vedi se arrivato badge
		 if (badge_present())
		   {
			  badge_get_last_reading(appl_logic_data.last_read_badge, sizeof(appl_logic_data.last_read_badge) - 1);
			  // inserire logica di gestione badge
			  // bla,bla,bla e se ok chiamo erog
		   }
		 // temporizzazione per lettura PC_time
		 if ((RTOS_get_tick_1ms() - PCtime_request_timer) >= PCTIME_REQUEST_RATE)
		   {
		      // funzione che legge il PCtime
			  // se OK chiamo la send_offline
			  PCtime_request_timer = RTOS_get_tick_1ms();
		   }
	  }
}

static void hand_ST_LOGIC_SEND_OFFLINE(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_send_offline)))
	{

	}
}

static void hand_ST_LOGIC_BADGE(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_badge)))
	{

	}
}

static void hand_ST_LOGIC_CHECK_LAN(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_check_lan)))
	  {
		if (server_LAN_connection())
		{
		   // 
			switch (FSM_Logica.old_state)
			  {
			     case ST_LOGIC_INIT:
				   appl_logic_data.flg_boot = FALSE;
				   fsm_set_state(&my_fsm_PLC, ST_LOGIC_INIT);
			       break;

			  }
		}
		else
		{
			//fsm_set_state(&my_fsm_PLC, ST_PLC_TEST_BADGE_LOCAL);
		}
	  }
}

static void hand_ST_LOGIC_HAND_ONLINE(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_hand_online)))
	{

	}
}

static void hand_ST_LOGIC_HAND_OFFLINE(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_hand_offline)))
	{

	}

}

static void hand_ST_LOGIC_EROG(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_erog)))
	{

	}
}

static void hand_ST_LOGIC_NON_TROVATO(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_non_trovato)))
	{

	}
}

static void hand_ST_LOGIC_RICHIESTA_AUTORIZZAZIONE(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_richiesta_autorizzazione)))
	{

	}
}

static void hand_ST_LOGIC_ERROR(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_error)))
	{

	}
}

static void hand_ST_LOGIC_FAIL(void)
{
	if (fsm_first_scan(&FSM_Logica, AVR_PGM_to_str(str_log_st_fail)))
	{

	}
}
#undef MODULE_APPL_LOGICA