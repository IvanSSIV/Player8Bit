

/****************************************************************************/
/* File: gestione_motore_nodenti.cpp                                        */
/* Data creazione: 20/04/2020  												*/
/* Creato da: Ivan De Stefani  												*/
/* Funzione: Modulo di gestione del motore senza sensore trasmissione.   	*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/
#define MODULE_APPL_MOTORE
#include "player_EC.h"

uint8_t sens_corrente = 0;					// pin A0, Digital46, PF0 per la lettura della corrente


// Define mnemoniche per gestione della macchina a stati del motore
FSM_WORK FSM_motor;

#define ST_MOT_INIT       0
#define ST_MOT_IDLE	      1
#define ST_MOT_STARTING   2
#define ST_MOT_ON	      3
#define ST_MOT_ERR	      4
#define ST_MOT_BLOCC      5
/*
static char* mot_state_name[] =
{
	"ST_MOT_INIT    ",
	"ST_MOT_IDLE    ",
	"ST_MOT_STARTING",
	"ST_MOT_ON      ",
	"ST_MOT_ERR     ",
	"ST_MOT_BLOCC   "
};
*/

static MOT_STAT motor_status;

// Definizione delle stringhe su Program Memory per il debug su seriale

const char str_mot_hand_init[] PROGMEM       = { "Handler degli eventi per il motore iniziato." };
const char str_mot_hand_end[] PROGMEM       = { "Handler degli eventi per il motore terminato." };
const char str_mot_init_start[] PROGMEM     = { "- Inizializzazione macchina a stati del motore -" };
const char str_mot_init_end[] PROGMEM       = { "- Fine inizializzazione macchina a stati del motore -" };
const char str_mot_send_event[] PROGMEM     = { "Motor send event : " };
const char str_mot_ev_none[] PROGMEM        = { "MOT_EV_NONE" };
const char str_mot_ev_start[] PROGMEM       = { "MOT_EV_START" };
const char str_mot_ev_stop[] PROGMEM        = { "MOT_EV_STOP" };
const char str_mot_ev_error[] PROGMEM       = { "MOT_EV_ERROR" };
const char str_mot_nessun_evento[] PROGMEM  = { "Nessun evento inviato al motore" };
const char str_mot_avvio_motore[] PROGMEM   = { "Inviato evento di avvio motore" };
const char str_mot_arresto_motore[] PROGMEM = { "Inviato evento di arresto motore" };
const char str_mot_errore_evento[] PROGMEM  = { "Errore di invio evento al motore!" };
const char str_mot_bloccato[] PROGMEM       = { "Motore bloccato per sovracorrente!" };
const char str_mot_st_init[] PROGMEM = { "ST_MOT_INIT     " };
const char str_mot_st_idle[] PROGMEM = { "ST_MOT_IDLE     " };
const char str_mot_st_starting[] PROGMEM = { "ST_MOT_STARTING " };
const char str_mot_st_on[] PROGMEM = { "ST_MOT_ON       " };
const char str_mot_st_err[] PROGMEM = { "ST_MOT_ERR      " };
const char str_mot_st_bloccato[] PROGMEM = { "ST_MOT_BLOCC    " };

// Vettore in program memory con i nomi degli stati
const char* const mot_state_name[] PROGMEM = {
	str_mot_st_init,
	str_mot_st_idle,
	str_mot_st_starting,
	str_mot_st_on,
	str_mot_st_err,
	str_mot_st_bloccato
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                                                     */
/*                                     FUNZIONI LOCALI AL MODULO                                       */
/*                                                                                                     */
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************/
/* function mot_ev_to_str()                                              */
/*_______________________________________________________________________*/
/* Gestisce l'associazione di stringhe di testo in program memory        */
/* agli eventi inviabili al server per la gestione del debug su seriale. */
/*                                                                       */
/* Parametri: event: evento da associare alla stringa                    */
/*************************************************************************/
static char* mot_ev_to_str(uint8_t event)
{
	char buff[20];
	switch (event)
	{
		case MOT_EV_NONE:
			strncpy(buff, AVR_PGM_to_str(str_mot_ev_none), sizeof(buff) - 1);
			break;
		case MOT_EV_START:
			strncpy(buff, AVR_PGM_to_str(str_mot_ev_start), sizeof(buff) - 1);
			break;
		case MOT_EV_STOP:
			strncpy(buff, AVR_PGM_to_str(str_mot_ev_stop), sizeof(buff) - 1);
			break;
		default:
			strncpy(buff, AVR_PGM_to_str(str_mot_ev_error), sizeof(buff) - 1);
			break;
	}
	return buff;
}

/*************************************************************************/
/* function mot_current()  || Verificare se fattibile su porta, PF0      */
/*_______________________________________________________________________*/
/* Effettua una lettura analogica del pin 0 per la corrente di taglio    */
/* del motore.                                                           */
/*                                                                       */
/* Parametri: nessuno                                                    */
/*************************************************************************/
static uint16_t mot_current(void)
{
	return analogRead(sens_corrente);
}

// Temporanea per test, eliminare dopo aver creato la procedura di lettura del config

static void temp_init_var(void)
{
	ritardo_taglio_motore = 300;
	taglio_motore = 22;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                                                     */
/*                                         FUNZIONI GLOBALI                                            */
/*                                                                                                     */
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/***************************************************/
/* function motor_init()                           */
/*_________________________________________________*/
/* Inizializza la struttura di tipo FSM_WORK per   */
/* la gestione del motore.                         */
/* Parametri: nessuno.                             */
/***************************************************/

void motor_init(void)
  {
     debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_mot_init_start);

	LL_motor_init();
	
	// Temporanea per test, eliminare dopo aver creato la procedura di lettura del config
	temp_init_var();
	
	fsm_init(&FSM_motor,
		ST_MOT_INIT,
		(void*)mot_state_name,
		ST_MOT_BLOCC,
		"MOT ");
	fsm_ena_print(&FSM_motor, mot_dbg, mot_dbg, mot_dbg);
	//fsm_set_timer(&FSM_motor, FSM_TIMER2);
	last_mot_event = MOT_EV_NONE;

        debug_message_timestamp_PGM(str_mot_init_end);

	/*  Funzione per restituire una struttura
	uint8_t stat;
	stat = motor_get_status()->err_code;
	*/
}

MOT_STAT *motor_get_status(void)
  {
	 motor_status.FSM_state = FSM_motor.state;

	 return &motor_status;
  }

/***************************************************/
/* function motor_send_event()                     */
/*_________________________________________________*/
/* Gestore degli eventi inviati al controllo del   */
/* motore.                                         */
/* Parametri: event                                */
/* retval                                          */
/*        0: nessun evento inviato                 */
/*        1: inviato start motore                  */
/*        2: inviato stop motore                   */
/*        3: errore nel parametro evento           */
/*        4: evento inviato in stato di BLOCCATO   */
/***************************************************/
uint8_t motor_send_event(uint8_t event)
{
	uint8_t retval = 0;

	#ifdef DEBUG_VERSION
		debug_print_timestamp(mot_dbg, AVR_PGM_to_str(str_mot_send_event));
		char buff[40];
		sprintf(buff, "[EVENT: %s] ", mot_ev_to_str(event));
		debug_print_ena(mot_dbg, buff);
	#endif

	if (FSM_motor.state != ST_MOT_BLOCC)
	{
		retval = event;
		switch (event)
		{
		case MOT_EV_NONE:
			#ifdef DEBUG_VERSION
				debug_print_ena(mot_dbg, AVR_PGM_to_str(str_mot_nessun_evento));
			#endif
			break;
		case MOT_EV_START:
			#ifdef DEBUG_VERSION
				debug_print_ena(mot_dbg, AVR_PGM_to_str(str_mot_avvio_motore));
			#endif
			//fsm_set_timer(&FSM_motor, FSM_TIMER1);
			fsm_set_state(&FSM_motor, ST_MOT_STARTING);
			break;
		case MOT_EV_STOP:
			#ifdef DEBUG_VERSION
				debug_print_ena(mot_dbg, AVR_PGM_to_str(str_mot_arresto_motore));
			#endif
			fsm_set_state(&FSM_motor, ST_MOT_IDLE);
			break;
		default:
			#ifdef DEBUG_VERSION
				debug_print_ena(mot_dbg, AVR_PGM_to_str(str_mot_errore_evento));
			#endif
			retval = MOT_EV_INVALID;
			fsm_set_state(&FSM_motor, ST_MOT_ERR);
		}
	}
	else
	{
		motor_status.err_code = MOT_EV_HALTED;
		retval = MOT_EV_HALTED;
	}
	return retval;
}


/***************************************************/
/* function motor_hand()                           */
/*_________________________________________________*/
/* Gestore del funzionamento ad alto livello       */
/* del motore.                                     */
/*                                                 */
/* Parametri: nessuno                              */
/***************************************************/
void motor_hand(void)
{
	switch (FSM_motor.state)
	{
	case ST_MOT_INIT:
		// "Ritardo" controllato per far vedere il passaggio dallo stato ST_MOT_INIT

		//if (fsm_timer_hand(&FSM_motor, FSM_TIMER2, 200L))
		{
			fsm_set_state(&FSM_motor, ST_MOT_IDLE);
		}

		break;

	case ST_MOT_IDLE:
		if (fsm_first_scan(&FSM_motor, AVR_PGM_to_str(str_mot_st_idle)))
		{
			motor_STOP(); // fermo il motore
		}
		break;

	case ST_MOT_STARTING:
		if (fsm_first_scan(&FSM_motor, AVR_PGM_to_str(str_mot_st_starting)))
		{
			motor_START(); // avvio il motore
			fsm_set_timer(&FSM_motor, FSM_TIMER1, ritardo_taglio_motore);
		}
		if (fsm_check_end_time(&FSM_motor, FSM_TIMER1))   // 300 ms di delay per attesa misurazione taglio corrente
		{
			fsm_set_state(&FSM_motor, ST_MOT_ON);
		}
		break;

	case ST_MOT_ON:
		uint16_t current;
		current = mot_current();
		if (current > taglio_motore)
		{
			#ifdef DEBUG_VERSION
				char buff[50];
				sprintf(buff, "current=%04u  taglio_motore=%04u", current, taglio_motore);
				debug_print_timestamp_title(mot_dbg, DEBUG_IDENT_L2, AVR_PGM_to_str(str_mot_bloccato), buff);
			#endif
			
			fsm_set_state(&FSM_motor, ST_MOT_BLOCC);
		}
		break;

	case ST_MOT_ERR:
		break;

	case ST_MOT_BLOCC:
		if (fsm_first_scan(&FSM_motor, AVR_PGM_to_str(str_mot_st_bloccato)))
		{
			motor_STOP();  // fermo il motore
		}
		break;

	default:
		;
	}
}

#undef MODULE_GEST_MOTORE_NODENTI