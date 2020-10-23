#ifdef MODULE_APPL_LOGICA
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif
// definizioni globali
// Enumerazione per gli stati della logica principale
enum main_logic_state 
  {
     //-- stati di inizializzazione e configurazione macchina
     ST_LOGIC_INIT = 0,                // Stato di inizializzazione
     ST_LOGIC_READ_CONFIG,             // legge config da (LAN o SD o EEPROM)
     ST_LOGIC_READ_ANAG,               // legge anagrafica da (LAN o SD o EEPROM)
     ST_LOGIC_FILL_STATUS,             // gestisce i file apertura,chiusura,anagrafica.dat
     ST_LOGIC_PCTIME,                  // legge l'ora dal server
     ST_LOGIC_SEND_OFFLINE,            // invio eventuali file offline
     //-------------------------------------------------------------------------------------------//
     //-- stati operativi	
     ST_LOGIC_MAIN,                     // Stato di decisioni operative delle macchina
     ST_LOGIC_FIND_ZERO,                // Ricerca dello zero dell'asse del tamburo per le macchine che lo richiedono
     ST_LOGIC_BADGE,			// Lettura del badge
     ST_LOGIC_HAND_ONLINE,		// Gestione autorizzazioni online
     ST_LOGIC_HAND_OFFLINE,		// Gestione autorizzazioni offline
     ST_LOGIC_EROG,		        // Erogazione dei tappi -- stato fortemente dipendente dall'applicazione
     ST_LOGIC_RICHIESTA_AUTORIZZAZIONE, // Richiesta autorizzazione al prelievo
	
     //-------------------------------------------------------------------------------------------//
     //-- stati di errore
     ST_LOGIC_INIT_ERROR,               // Uno degli stati di inizializzazione ha fallito
     ST_LOGIC_ERROR,                    // Errore nella logica in runtime
     ST_LOGIC_FAIL			// Failure non recuperabile (da hardware)
  };
typedef enum main_logic_state MAIN_LOGIC_STATE;

// enumerazione per i codici di errore della macchina a stati principale
enum main_logic_error
  {
     ERR_LOGIC_NONE = 0  
  };
typedef enum main_logic_error MAIN_LOGIC_ERROR;

// enumerazione per i codici di failure della macchina a stati principale
enum main_logic_fail
  {
     FAIL_LOGIC_NONE = 0,   // condizione normale
     FAIL_LOGIC_HW,         // errore hw non recuperabile
     FAIL_LOGIC_MACHINE_ID, // ID macchina non definito
     FAIL_LOGIC_NO_CONFIG,  // config non trovato
     FAIL_LOGIC_INT_ERROR   // errore interno (condizione non prevista)
  };
typedef enum main_logic_fail MAIN_LOGIC_FAIL;

// prototipi funzione globali
void logic_init(void);
void logic_main(void);

#undef _EXTVAR