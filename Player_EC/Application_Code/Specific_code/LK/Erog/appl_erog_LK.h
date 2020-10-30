#ifdef MODULE_APPL_EROG
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

enum LK_erog_state
{
  ST_EROG_INIT = 0,
  ST_EROG_IDLE,
  ST_EROG_START,
  ST_EROG_SELECT_MODE,
  ST_EROG_PRELIEVO,
  ST_EROG_RESO,
  ST_EROG_REFILL,
  ST_EROG_DONE,
  ST_EROG_ERROR,
  ST_EROG_FAIL
};


#undef _EXTVAR