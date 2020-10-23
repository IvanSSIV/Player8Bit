
// Possibili valori di ritorno da un funzione di stato di una FSM
enum func_FSM_retval 
{
    FSM_NOT_INIT = 0, // attivita non inizializzata
    FSM_READY,        // pronto ad eseguire attivita
    FSM_EXEC,         // attivita in corso
    FSM_DONE,         // attivita eseguita correttamente
    FSM_ERROR,        // avvenuto errore
    FSM_TIMEOUT,      // timeout esecuzione stato
    FSM_FAIL,         // errore fatale (non recuperabile) tipicamente HW
    FSM_INVALID_STATE // errore interno (non previsto) baco .... 
};
typedef enum func_FSM_retval FSM_RETVAL;

// possibili valori di ritorno di una generica funzione
// con restituzione di un esito
enum func_retval
  {
     FUNC_OK = 1,     // attivita eseguita correttamente
     FUNC_ERROR,        // avvenuto errore
     FUNC_FAIL,         // errore fatale (non recuperabile) tipicamente HW
     FUNC_INVALID_STATE // erroe interno (non previsto) baco .... 
  };
typedef enum func_retval FUNC_RETVAL;

// Definizione di tipo per memorizzazione indirizzi IP
struct ip_addr
{
    uint8_t oct1;
    uint8_t oct2;
    uint8_t oct3;
    uint8_t oct4;
};
typedef struct ip_addr MY_IP_ADDR;

// definizione dispositivi di salvataggio dati
#define MEDIA_SD      1
#define MEDIA_USB     2
#define MEDIA_EXT_DEV 3
