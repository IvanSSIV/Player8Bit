#ifdef MODULE_GLOBAL_VAR
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

/*-----------------------------------------------------------------------*/
/* "Foto" delle principali variabili globali contenente le informazioni  */
/* più importanti sullo stato della macchina                             */
/* E' la struttura globale delle veriabili che devono essere visibili in */
/* tutti i moduli dell'applicazione                                      */
/* Contiene le variabili globali generiche , presenti in tutte           */
/* le applicazioni                                                       */
/*-----------------------------------------------------------------------*/
struct globvar
  {
     uint8_t  machine_id;  // definizione tipo di macchina (XL,EC,ecc)
     LANZI_HW_ID      harwware_id; // definizione modello di scheda

     BOOL    flg_boot;            // primo passaggio dopo reset
     BOOL    flg_LAN_OK;          // esito test ultima connessione
     BOOL    flg_LAN_first;       // avvenuta prima connessione

     BOOL    flg_config_OK;       // config OK (risolutivo)
     BOOL    flg_serv_config_OK;  // config letto dal server OK
     BOOL    flg_SD_config_OK;    // config su SD OK
     BOOL    flg_EEP_config_OK;   // config in EEPROM OK

     BOOL    flg_anag_OK;         // anagrafica OK (risolutivo)
     BOOL    flg_serv_anag_OK;    // anagrafica letta da server OK
     BOOL    flg_SD_anag_OK;      // anagrafica su SD OK
     BOOL    flg_EEP_anag_OK;     // anagrafica in EEPROM OK
     
     BOOL    flg_barcode_read;    // flag avvenuta lettura barcode
     BOOL    flg_read_PCtime_req; // scaduto tempo per lettura PCtime
     uint8_t FSM_error_code;      // codice errore FSM principale
     uint8_t FSM_fail_code;       // codice failure FSM principale
 
     BOOL    LAN_hw_fail;         // errore hardware LAN
     BOOL    RTC_hw_fail;         // errore hardware RTC
     BOOL    SD_hw_fail;          // errore hardware SD
     
     // Flag di abilitazione al debug di stato, attivabili/disattivabili in runtime
     BOOL    dbg_ena_ST_LOGIC_INIT;
     BOOL    dbg_ena_ST_LOGIC_READ_CONFIG;
     BOOL    dbg_ena_ST_LOGIC_READ_ANAG;
     BOOL    dbg_ena_ST_LOGIC_FILL_STATUS;
     BOOL    dbg_ena_ST_LOGIC_PCTIME;
     BOOL    dbg_ena_ST_LOGIC_SEND_OFFLINE;
     BOOL    dbg_ena_ST_LOGIC_MAIN;
     BOOL    dbg_ena_ST_LOGIC_FIND_ZERO;
     BOOL    dbg_ena_ST_LOGIC_BADGE;
     BOOL    dbg_ena_ST_LOGIC_HAND_ONLINE;
     BOOL    dbg_ena_ST_LOGIC_HAND_OFFLINE;
     BOOL    dbg_ena_ST_LOGIC_EROG;
     BOOL    dbg_ena_ST_LOGIC_RICHIESTA_AUTORIZZAZIONE;
     BOOL    dbg_ena_ST_LOGIC_INIT_ERROR;
     BOOL    dbg_ena_ST_LOGIC_ERROR;
     BOOL    dbg_ena_ST_LOGIC_FAIL;
  };
typedef struct globvar GLOBVAR;

/*------------------------------------------------------------*/
/* struttura con variabili globali specifiche di applicazione */
/*------------------------------------------------------------*/
struct app_globvar
  {
     uint8_t tappi;            // Numero di tappi caduti
     BOOL    deliver_auth;        // Autorizzazione alla consegna per presenza mano
  };
typedef struct app_globvar APP_GLOBVAR;

// definizione variabili globali
_EXTVAR GLOBVAR     global_data;      // struttura variabili globali (generiche)
_EXTVAR APP_GLOBVAR app_globdata;     // struttura variabili globali (di applicazione)
_EXTVAR PLC_IO      PLC_IO_data;      // struttura I/O del PLC

// prototipi funzione globali
void global_data_init(void);

#undef _EXTVAR
