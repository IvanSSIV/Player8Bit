// modulo con definzione di costanti,strutture,ecc specifiche
// per applicazioni PLC

#define PLC_MAX_X  24
#define PLC_MAX_Y  24

struct PLC_work
  {
     BOOL X[PLC_MAX_X];
     BOOL Y[PLC_MAX_Y];
  };
typedef struct PLC_work PLC_WORK;