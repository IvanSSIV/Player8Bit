// modulo con definzione di costanti,strutture,ecc specifiche
// per applicazioni PLC


struct PLC_IO_var
  {
     BOOL X[PLC_MAX_X];
     BOOL Y[PLC_MAX_Y];
  };
typedef struct PLC_IO_var PLC_IO;