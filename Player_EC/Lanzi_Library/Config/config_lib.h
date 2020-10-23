#ifdef MODULE_CONFIG_LIB
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

_EXTVAR CONFIG_DAT  setup_macchina; // setup 
_EXTVAR CONFIG_WORK config_work;   // dati 

void config_init_all(void);
void config_main_all(void);
BOOL verifica_congruita_config_ricevuto(void);


#undef _EXTVAR