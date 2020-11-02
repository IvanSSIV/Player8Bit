#ifdef MODULE_TIMING
#define  _EXTVAR /**/
#else
#define  _EXTVAR extern
#endif

// definizioni globali

// definizione variabili
_EXTVAR BOOL  flg_bt_10ms;   // flag base tempi a 10ms
_EXTVAR BOOL  flg_bt_100ms;  // flag base tempi a 100ms
_EXTVAR BOOL  flg_bt_1s;     // flag base tempi a 1sec

// prototipi funzione
uint32_t RTOS_get_tick_1ms(void);
void basetempi_RTOS_init(void);
void basetempi_RTOS_main(void);
void basetempi_RTOS_clear_flg(void);
#undef _EXTVAR