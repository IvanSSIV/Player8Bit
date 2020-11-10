
#ifdef MODULE_APPL_MOTORE
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif

struct motor_status
{
	uint8_t FSM_state;
	uint8_t err_code;
};
typedef struct motor_status MOT_STAT;

#define MOT_EV_NONE	    0
#define MOT_EV_START_FWD    1
#define MOT_EV_START_REV    2
#define MOT_EV_STOP	    3
#define MOT_EV_INVALID      4  // evento non valido
#define MOT_EV_HALTED       5  // motore bloccate

enum motor_state {
  ST_MOT_INIT = 0,
  ST_MOT_IDLE,
  ST_MOT_STARTING_FWD,
  ST_MOT_STARTING_REV,
  ST_MOT_ON_FWD,
  ST_MOT_ON_REV,
  ST_MOT_ERR,
  ST_MOT_BLOCC  
};

_EXTVAR uint8_t last_mot_event;		// Variabile per la gestione del funzionamento del motore con invio di eventi al suo gestore
_EXTVAR uint32_t ritardo_taglio_motore;				// millisecondi dopo i quali la corrente del motore viene misurata
_EXTVAR uint16_t taglio_motore;						// lettura analogica del pin 0 per la sovracorrente del motore

void motor_init(void);
uint8_t motor_send_event(uint8_t event);
void motor_hand(void);
MOT_STAT *motor_get_status(void);
#undef _EXTVAR