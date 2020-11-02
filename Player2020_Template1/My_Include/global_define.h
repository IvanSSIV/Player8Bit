
// possibili valori di ritorno da un generico gestore di eventi
enum EVENT_HAND_RETVAL 
{
    EVENT_HAND_STARTING = 1, // Gestore non ancora inizializzato
    EVENT_HAND_READY,        // gestore pronto a ricevere comandi
    EVENT_HAND_EXEC,         // gestore in esecuzione di un comando
    EVENT_HAND_DONE,         // comando eseguito correttamente
    EVENT_HAND_ERROR,        // errore nell'esecuzione del comando
    EVENT_HAND_FAIL          // errore fatale (non recuperabile)
};
typedef enum EVENT_HAND_RETVAL event_hand_retval;