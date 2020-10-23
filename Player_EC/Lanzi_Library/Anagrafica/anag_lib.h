#ifdef MODULE_ANAG_LIB
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

_EXTVAR ANAG_WORK anag_work;   // dati 

void anag_data_init(void);


#undef _EXTVAR
