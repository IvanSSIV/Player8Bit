/**********************************************************/
/* Modulo per l'interfaccia fra logica PLC e logica */
/* gestione server.*/
/* Viene chiamata tipicamente da uno stato della macchina */
/* a stati principale*/
/* Si occupa di passare una richiesta di servizio verso */
/* il server, richiesta che viene gestita dalla */
/* Server_request_hand nel modulo appl_server.*/

// include comuni in libreria "GP_library"
#include "..\GP_library.h"


// Stringhe in program memory per messaggi di debug
const char str_serv_dial_start[] PROGMEM = { "[sub SERV_DIALOG_START]" };
const char str_serv_dial_init_retry[] PROGMEM = { "[sub SERV_DIALOG_INIT_RETRY]" };
const char str_serv_dial_wait[] PROGMEM = { "[sub SERV_DIALOG_WAIT]" };
const char str_serv_finito[] PROGMEM = { "finito ..." };
const char str_serv_dial_done[] PROGMEM = { "[sub SERV_DIALOG_DONE]" };


// definizione struttura con i dati di modulo
struct PLC_service_data
  {
     TINY_FSM       fsm;          // FSM per gestione dialogo con server hander 
     uint8_t        service_ID;   // appoggio ID della richiesta corrente
  };
typedef struct PLC_service_data PLC_SERVICE_DATA;

// definizioni variabili locali
static PLC_SERVICE_DATA PLC_service_data;

#define SERV_DIALOG_INIT       0  // condizione iniziale all'accensione
#define SERV_DIALOG_WAIT       1  // condizione di attesa (tipicamente non fa nulla)
#define SERV_DIALOG_START      2  // avvia richiesta al gestore del server
#define SERV_DIALOG_INIT_RETRY 3  // attesa retry se gestore del server occupato
#define SERV_DIALOG_SERV_WAIT  4  // servizo in esecuzione/attesa completamento
#define SERV_DIALOG_DONE       5  // operazione completata


/*-------------------------------------------------------*/
/* Avvia la richiesta di un generico servizo al server   */
/* Riceve l'ID del servizio da richiedere                */
/* Deve essere chiamata one-shot da una macchina a stati */
/* per avviare una richiesta . In ingresso riceve        */
/* l'ID del servizio da richiedere                       */
/*-------------------------------------------------------*/
void SRV_service_request_start(uint8_t service_ID)
{
    PLC_service_data.service_ID = service_ID;
    PLC_service_data.fsm.first = TRUE;
    PLC_service_data.fsm.stat = SERV_DIALOG_START;
}

/*-------------------------------------------------------*/
/* Gestisce l'esecuzione della richiesta avviata dalla   */
/* "SRV_service_request_start".                          */
/* Restituisce TRUE al termine dell'operazione a         */
/* prescindere dall'esito (quando il gestore del server  */
/* torna "ready"                                         */
/* Deve essere richiamata continuamente (all'interno di  */
/* uno stato della macchina a stati principale) .        */
/*-------------------------------------------------------*/
FSM_RETVAL SRV_service_request_hand(void)
{
    FSM_RETVAL retval;

    retval = FSM_EXEC;
    switch (PLC_service_data.fsm.stat)
    {
    case SERV_DIALOG_INIT:
        PLC_service_data.fsm.stat = SERV_DIALOG_WAIT;
        break; 
    // condizione di attesa
    case SERV_DIALOG_WAIT:
        break;
    // avvio riciesta di servizio
    case SERV_DIALOG_START:
        if (PLC_service_data.fsm.first)
          {
            PLC_service_data.fsm.first = FALSE;

            #ifdef DEBUG_VERSION
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L2, AVR_PGM_to_str(str_serv_dial_start));
            #endif
          }
        // verifico che il gestore del server sia pronto a eseguire
        if (server_request_state() == FSM_READY)
        {
            // invio un evento al gestore
            server_request_send((server_msg_id) PLC_service_data.service_ID, "");
            PLC_service_data.fsm.first = TRUE;
            PLC_service_data.fsm.stat = SERV_DIALOG_SERV_WAIT;
        }
        else
        {
             FSMTINY_set_timer(&PLC_service_data.fsm);
            PLC_service_data.fsm.first = TRUE;
            PLC_service_data.fsm.stat = SERV_DIALOG_INIT_RETRY;
        }
        break;
    // attesa se server-handler e' occupato
    case SERV_DIALOG_INIT_RETRY:
        if (PLC_service_data.fsm.first)
        {
            PLC_service_data.fsm.first = FALSE;

            #ifdef DEBUG_VERSION
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L2, AVR_PGM_to_str(str_serv_dial_init_retry));
            #endif
        }
        if (FSMTINY_timer_EOT(&PLC_service_data.fsm, 1000L))
        {
            PLC_service_data.fsm.stat = SERV_DIALOG_START;
        }
        break;

    // richiesta di servizio inviata , attesa completamento operazioni
    case SERV_DIALOG_SERV_WAIT:
        static BOOL first_done;
        static uint16_t exec_count;
        if (PLC_service_data.fsm.first)
        {
            first_done = TRUE;
            PLC_service_data.fsm.first = FALSE;
            exec_count = 0;
            #ifdef DEBUG_VERSION
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L2, AVR_PGM_to_str(str_serv_dial_wait));
            #endif
        }
        FSM_RETVAL esito;
        // leggi lo stato di avanzamento della richiesta di servizio
        esito = server_request_state();
        switch (esito)
        {
        case FSM_EXEC:
            exec_count++;
            break;
        case FSM_DONE:
            if (first_done)
            {
                first_done = FALSE;

                #ifdef DEBUG_VERSION
                    debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L2, AVR_PGM_to_str(str_serv_finito));
                    char buff[80];
                    sprintf(buff, "exec count=%05u", exec_count);
                    debug_print_timestamp(DBG_ALWAYS_ON, buff);
                #endif
            }
            break;
        case FSM_READY:
            PLC_service_data.fsm.first = TRUE;
            PLC_service_data.fsm.stat = SERV_DIALOG_DONE;
            break;
        case FSM_ERROR:
            FSMTINY_set_timer(&PLC_service_data.fsm);
            PLC_service_data.fsm.first = TRUE;
            PLC_service_data.fsm.stat = SERV_DIALOG_INIT_RETRY;
            break;
        case FSM_FAIL:
            break;
        }
        break;

    case SERV_DIALOG_DONE:
        if (PLC_service_data.fsm.first)
        {
            PLC_service_data.fsm.first = FALSE;
            
            #ifdef DEBUG_VERSION
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L2, AVR_PGM_to_str(str_serv_dial_done));
            #endif
        }
        PLC_service_data.fsm.stat = SERV_DIALOG_WAIT;
        retval = FSM_DONE;
        break;

    }

    return retval;
}