#define MODULE_ANAG_LIB
// include comune librerie LANZI
#include "../Lanzi_library.h"

/*----------------------------------------------------------*/
/* Inizializza la struttura di lavoro con le variabili per  */
/* la gestione dell'anagrafica                              */
/*----------------------------------------------------------*/
void anag_data_init(void)
  {
     memset(&anag_work, 0, sizeof(anag_work));
     anag_work.dbg_ena = TRUE;
     anag_work.dbg_idn = 2;
  }

#undef MODULE_ANAG_LIB
  