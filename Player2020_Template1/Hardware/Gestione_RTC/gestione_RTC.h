
// prototipi funzione locali
void RTC_init_LL(void);                                       // inizializzazione di modulo
BOOL RTC_gettime_low_level(void);
void RTC_gettime_HL(RTC_WORK* time_buff);
void RTC_settime_LL(RTC_WORK* time_buff);