// definizione stati gestione operazioni su SD
#define ST_STORAGE_INIT              0  // condizione iniziale
#define ST_STORAGE_WAIT_CMD          1  // attesa comandi
#define ST_STORAGE_CHK_APERTURA      2  // verifica "apertura.dat"
#define ST_STORAGE_CHK_CHIUSURA      3  // verifica "chiusura.dat"
#define ST_STORAGE_CHK_RICARICA      4  // verifica "ricarica.dat"
#define ST_STORAGE_READ_RICARICA     5  // leggi "ricarica.dat"
#define ST_STORAGE_FIND_BADGE        6  // cerca un badge
#define ST_STORAGE_READ_ABIL         7  // leggi abilitazioni
#define ST_STORAGE_SOFT_ERR        253  // caso non previsto
#define ST_STORAGE_FAIL            254  // failure HW non ricuperabile
#define ST_STORAGE_ERROR           255  // errore

// definizione comandi/risposte verso gestore memorizzazioni
#define CMD_STORAGE_CHK_APERTURA   0x10  // verifica "apertura.dat"
#define CMD_STORAGE_CHK_CHIUSURA   0x11  // verifica "chiusura.dat"
#define CMD_STORAGE_CHK_RICARICA   0x12  // verifica "ricarica.dat"
#define CMD_STORAGE_READ_RICARICA  0x13  // leggi "ricarica.dat"
#define CMD_STORAGE_FIND_BADGE     0x14  // cerca il badge (in ANAG.DAT)
#define CMD_STORAGE_READ_ABIL      0x15  // leggi abilitazioni del badge corrente
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
 };
typedef struct storage_work_data STORAGE_WORK;

