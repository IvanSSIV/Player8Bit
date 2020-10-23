
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

#define MOT_EV_NONE			0
#define MOT_EV_START		1
#define MOT_EV_STOP			2
#define MOT_EV_INVALID      3  // evento non valido
#define MOT_EV_HALTED        4  // motore bloccate

_EXTVAR uint8_t last_mot_event;		// Variabile per la gestione del funzionamento del motore con invio di eventi al suo gestore
_EXTVAR uint32_t ritardo_taglio_motore;				// millisecondi dopo i quali la corrente del motore viene misurata
_EXTVAR uint16_t taglio_motore;						// lettura analogica del pin 0 per la sovracorrente del motore

void motor_init(void);
uint8_t motor_send_event(uint8_t event);
void motor_hand(void);
MOT_STAT *motor_get_status(void);
#undef _EXTVAR