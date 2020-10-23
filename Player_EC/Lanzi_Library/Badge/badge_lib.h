
// Enumerazione per gli stati di gestione
// del riconoscimento utente tramite badge

#define MAX_BADGE_STATE		ST_BADGE_FAIL
#define MAX_BADGE_SIZE		20

// valore restituito dalla read_char
// quando non vi e' nulla in lettura
#define BADGE_INVALID_CHR   0xFFFF

enum badge_state {
	ST_BADGE_INIT = 0, 
	ST_BADGE_READY,
	ST_BADGE_READING,
	ST_BADGE_DONE,
	ST_BADGE_ERROR,
	ST_BADGE_FAIL
};

// prototipi funzione globali
void badge_init(void);
void badge_hand(void);
void badge_ena_read(void);
void badge_dis_read(void);
BOOL badge_present(void);
void badge_get_last_reading(char* read_badge, uint8_t size);
