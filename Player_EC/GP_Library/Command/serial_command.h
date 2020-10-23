#ifdef MODULE_SERIAL_COMMAND
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif // MODULE_SERIAL_COMMAND

// Enumerazione per gli stati gestione sequenze di ESC
enum ESC_hand_state 
  {
     //-- stati di inizializzazione e configurazione macchina
     ST_ESC_WAIT = 0,          // attesa arrivo ESC
     ST_ESC_FIRST,             // ricevuto primo carattere
     ST_ESC_SECOND,               // legge anagrafica da (LAN o SD o EEPROM)
  };
typedef enum ESC_hand_state ESC_HAND_STATE;


void serial_ESC_command(void);
BOOL serial_ESC_player(char c);
BOOL serial_ESC_LANZI(char c);
BOOL serial_ESC_system(char c);
#undef _EXTVAR