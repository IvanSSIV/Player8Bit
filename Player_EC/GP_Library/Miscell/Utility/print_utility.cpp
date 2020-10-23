#include "../../GP_library.h"

static const char str_riga_separaz[] PROGMEM  = { "\n*------------------------------*" };   

// prototipi funzioni esterne (in appl_main.cpp)
extern const char *get_nome_apparato(void);
extern const char *get_versione_FW(void);

void appl_init_title(void)
  {
     const char *nome_apparato;
     const char *versione_FW;

     nome_apparato = get_nome_apparato();
     versione_FW = get_versione_FW();

     debug_print((char*)"\n\r");
     debug_print(AVR_PGM_to_str(str_riga_separaz));
     debug_print(AVR_PGM_to_str(nome_apparato));
     debug_print(AVR_PGM_to_str(versione_FW));
     debug_print(AVR_PGM_to_str(str_riga_separaz));
  }
