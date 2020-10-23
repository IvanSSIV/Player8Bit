// Definizione della struttura di gestione della macchina a stati minimale
struct tiny_FSM
{
    uint8_t  stat;
    BOOL     first;
    uint32_t timer;
};
typedef struct tiny_FSM TINY_FSM;

// Prototipi di funzione
void FSMTINY_set_timer(TINY_FSM* fsm,uint32_t timer_val);
BOOL FSMTINY_check_EOT(TINY_FSM* ptr);
void FSMTINY_init(TINY_FSM* ptr, uint8_t init_stat);
void FSMTINY_set_state(TINY_FSM* ptr, uint8_t new_stat);
BOOL FSMTINY_first(TINY_FSM* ptr);