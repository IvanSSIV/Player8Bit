#ifdef MODULE_APPL_LOGICA
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

// Enumerazione per gli stati della logica principale

enum logic_state {
	ST_LOGIC_INIT = 0,                        // Stato di inizializzazione
	ST_LOGIC_MAIN,							  // Stato di attesa passaggio badge
	ST_LOGIC_SEND_OFFLINE,					  // Richiesta PCTime e invio eventuali file offline
	ST_LOGIC_BADGE,							  // Lettura del badge
	ST_LOGIC_CHECK_LAN,						  // Verifica stato della connessione
	ST_LOGIC_HAND_ONLINE,				      // Gestione autorizzazioni online
	ST_LOGIC_HAND_OFFLINE,					  // Gestione autorizzazioni offline
	ST_LOGIC_EROG,						      // Erogazione dei tappi
	ST_LOGIC_NON_TROVATO,					  // Badge non trovato nell'anagrafica online o offline
	ST_LOGIC_RICHIESTA_AUTORIZZAZIONE,		  // Richiesta autorizzazione al prelievo
	ST_LOGIC_ERROR,							  // Errore nella logica
	ST_LOGIC_FAIL							  // Failure non recuperabile
};

void logic_init(void);
void logic_main(void);

#undef _EXTVAR