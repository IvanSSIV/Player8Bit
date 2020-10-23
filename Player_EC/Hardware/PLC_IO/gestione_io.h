// gestione_io.h

// definizioni globali


// prototipi funzione globali
void GPIO_init(void);
void PLC_read_input(PLC_IO *plc);
void PLC_write_output(PLC_IO *plc);
void PLC_print_state(BOOL ena, PLC_IO * lc);
