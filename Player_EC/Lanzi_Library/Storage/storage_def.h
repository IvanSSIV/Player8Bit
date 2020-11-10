// definizione stati gestione operazioni su SD


enum storage_state {
  ST_STORAGE_INIT = 0,      // condizione iniziale
  ST_STORAGE_WAIT_CMD,      // attesa comandi
  ST_STORAGE_CHK_APERTURA,  // verifica "apertura.dat"
  ST_STORAGE_CHK_CHIUSURA,  // verifica "chiusura.dat"
  ST_STORAGE_CHK_RICARICA,  // verifica "ricarica.dat"
  ST_STORAGE_READ_RICARICA, // leggi "ricarica.dat"
  ST_STORAGE_FIND_BADGE,    // cerca un badge
  ST_STORAGE_READ_ABIL,     // leggi abilitazioni
  ST_STORAGE_WRITE_MV,      // Scrittura del file per il salvataggio su SD delle transazioni
  ST_STORAGE_WRITE_OFFLINE, // Scrittura dei file offline per invio transazioni al server
  ST_STORAGE_SOFT_ERR,      // caso non previsto
  ST_STORAGE_FAIL,          // failure HW non ricuperabile
  ST_STORAGE_ERROR          // errore
};

// definizione comandi/risposte verso gestore memorizzazioni
#define CMD_STORAGE_CHK_APERTURA   0x10  // verifica "apertura.dat"
#define CMD_STORAGE_CHK_CHIUSURA   0x11  // verifica "chiusura.dat"
#define CMD_STORAGE_CHK_RICARICA   0x12  // verifica "ricarica.dat"
#define CMD_STORAGE_READ_RICARICA  0x13  // leggi "ricarica.dat"
#define CMD_STORAGE_FIND_BADGE     0x14  // cerca il badge (in ANAG.DAT)
#define CMD_STORAGE_READ_ABIL      0x15  // leggi abilitazioni del badge corrente
#define CMD_STORAGE_SAVE_TRANS     0x16  // Salva transazione
#define CMD_STORAGE_SAVE_OFFLINE   0x17  // Salva file offline
#define ANS_STORAGE_OK             0x40  // comando OK

#define STORAGE_CMD_SIZE 32  // dimensione buffer comandi
#define STORAGE_ANS_SIZE 32  // dimensione buffer risposte


// struttura dati di lavoro per gestione storage
struct storage_work_data
  {
     uint8_t stat;                            // stato corrente
     bool    first;                           // flag prima scansione nello stato
     BOOL    debug_ena;                       // flag di abilitazione del debug
     uint8_t debug_ident;                     // livello di identazione per stampa
     BOOL    boot_flg;                        // flag prima scansione dopo reset
     uint8_t command;                         // comando da eseguire 
     uint8_t answer;                          // risposta al comando
     uint8_t command_data[STORAGE_CMD_SIZE];  // buffer dati comando
     uint8_t answer_data[STORAGE_ANS_SIZE];   // buffer dati risposta
     char    msg_data[120];                   // Dati del messaggio da inserire nei file offline o MV
 };
typedef struct storage_work_data STORAGE_WORK;

