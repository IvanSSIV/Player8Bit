#ifdef MODULE_SERIAL_COMMAND_DEBUG
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif // MODULE_SERIAL_COMMAND

_EXTVAR bool rtc_dbg;		// Flag per debug RTC
_EXTVAR bool  sd_dbg;		// Flag per debug SD
_EXTVAR bool lcd_dbg;		// Flag per debug LCD
_EXTVAR bool eth_dbg;		// Flag per debug ETHERNET
_EXTVAR bool srv_dbg;		// Flag per debug SERVER
_EXTVAR bool mot_dbg;		// Flag per debug MOTORE
_EXTVAR bool lgc_dbg;		// Flag per debug LOGICA
_EXTVAR bool plc_dbg;		// Flag per debug PLC
_EXTVAR bool eep_dbg;		// Flag per debug EEPROM
_EXTVAR bool bdg_dbg;		// Flag per debug BADGE
_EXTVAR bool ser_dbg;		// Flag per debug COMANDI DA SERIALE
_EXTVAR bool  io_dbg;		// Flag per debug GPIO
_EXTVAR bool fsm_dbg;		// Flag per debug FSM
_EXTVAR bool erg_dbg;		// Flag per debug EROGAZIONE

// prototipi funzione globali
BOOL serial_ESC_debug(char c);
void serial_debug_init(void);

#undef _EXTVAR