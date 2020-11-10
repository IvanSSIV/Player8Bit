enum gr_erog_state {
  ST_EROG_IDLE = 0,
  ST_EROG_APRI_SPORTELLO,
  ST_EROG_WAIT_SPORTELLO_CHIUSO,
  ST_EROG_CHECK_DEPOSITO,
  ST_EROG_APRI_BOTOLA,
  ST_EROG_CHIUDI_BOTOLA,
  ST_EROG_CHECK_CADUTA,
  ST_EROG_OK,
  ST_EROG_KO,
  ST_EROG_ERROR
};

BOOL erog_is_ready(void);
void erog_start(void);
void erog_stop(void);
void erog_abort(void);
BOOL erog_is_OK(void);
BOOL erog_is_error(void);
void erog_reset_error();
void erog_init(void);
void erog_hand(void);