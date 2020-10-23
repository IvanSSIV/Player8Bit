//#include "mydef.h"
struct FSM_work
  {
     uint8_t  state;           // stato corrente
     uint8_t  old_state;       // stato precente
     BOOL     first;           // flag prima scansione
     BOOL     ena_print_chg;   // abilita stampa cambio stato
     BOOL     ena_print_first; // abilita stampa prima scansione
     BOOL     ena_crlf_chg;    // abilita stampa CR+LF 
     uint8_t  max_state;       // numero massimo stati
     #ifdef ENV_AVR
     void     *state_name;    // array nome degli stati
    #else   
    char      **state_name;    // array nome degli stati
    #endif
     char     name[5];         // nome della macchina a stati
     uint32_t delay_1;   // variabile per ritardo
     uint32_t delay_2;   // variabile per ritardo
     uint32_t delay_3;   // variabile per ritardo
     uint32_t delay_4;   // variabile per ritardo
  };
typedef struct FSM_work FSM_WORK;

struct FSM_work_light
{
    uint8_t  state;           // stato corrente
    uint8_t  old_state;       // stato precente
    BOOL     first;           // flag prima scansione
    BOOL     ena_print_chg;   // abilita stampa cambio stato
    BOOL     ena_print_first; // abilita stampa prima scansione
    uint8_t  max_state;       // numero massimo stati
    #ifdef ENV_AVR
    const char** const   state_name;    // array nome degli stati
    #else   
    char**               state_name;    // array nome degli stati
    #endif
    uint16_t delay_1;   // variabile per ritardo
    uint16_t delay_2;   // variabile per ritardo
    uint16_t delay_3;   // variabile per ritardo
    uint16_t delay_4;   // variabile per ritardo
};
typedef struct FSM_work_light FSMLIGHT_WORK;


// definizione mnemonica timer di stato
#define FSM_TIMER1 1
#define FSM_TIMER2 2
#define FSM_TIMER3 3
#define FSM_TIMER4 4

#define MAX_MSG_STATE_SIZE 24
// 
#ifdef ENV_AVR
void fsm_init(FSM_WORK* fsm, uint8_t initstat, void * state_name, uint8_t max_state, char* name);
#else
void fsm_init(FSM_WORK* fsm, uint8_t initstat, char** state_name, uint8_t max_state, char* name);
#endif
void fsm_set_state(FSM_WORK* fsm, uint8_t new_state);
void fsm_ena_print(FSM_WORK* fsm, BOOL chg, BOOL first,BOOL crlf);
BOOL fsm_first_scan(FSM_WORK* fsm, char* message);
void fsm_set_timer(FSM_WORK* fsm, uint8_t timer_num,uint32_t set_value);
BOOL fsm_check_end_time(FSM_WORK* fsm, uint8_t timer_num);