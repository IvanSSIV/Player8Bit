/*****************************************************************************************************/
#ifdef MODULE_APPL_EROG
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

#define MOTOR_TEST_DELAY     3000L      // Millisecondi per i quali il motore rimane acceso durante la fase di test
#define DELIVER_TIMEOUT     30000L		// Timeout per la consegna dei tappi
#define EROG_END_TIME		 2000L		// Dopo 2000 ms, la macchina per la consegna torna in START


// Enumerazione per i sottostati della macchina di gestione della consegna dei tappi
enum erog_state {
	ST_EROG_IDLE = 0,	// Stato di attesa
	ST_EROG_START, 	        // Avvio macchina a stati; set timer
	ST_EROG_WAITING_MANO,	// Attesa inserimento della mano
	ST_EROG_WAITING_T1,	// Attesa caduta primo tappo
	ST_EROG_WAITING_T2,	// Attesa caduta secondo tappo
	ST_EROG_OK,		// Erogazione andata a buon fine
	ST_EROG_KO		// Erogazione fallita per timeout
};

// Enumerazione per gli eventi della macchina di gestione dell'erogazione
enum erog_event {
	EROG_EV_BDG_OK = 0					// Badge OK
};

void erog_init(void);
void erog_hand(void);
void erog_event_send(uint8_t EROG_EV_ID);
uint16_t erog_sizeof_state_message(void);
BOOL erog_is_ready(void);
void erog_start(void);
void erog_stop(void);
void erog_abort(void);
BOOL erog_is_OK(void);
void erog_reset_error();

#undef _EXTVAR