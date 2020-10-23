
struct RTC_work
{
    uint8_t day;        // giorno del mese
    uint8_t month;      // mese
    uint16_t year;       // anno (a cui aggiungere 2000)

    uint8_t hour;       // ore
    uint8_t min;        // minuti
    uint8_t sec;        // secondi

    uint8_t dow;        // giorno della settimana
};
typedef struct RTC_work RTC_WORK;

enum stato_RTC    // Enumerazione per gli stati della macchina di gestione dell'RTC
{
    RTC_ST_INIT = 0,
    RTC_ST_FIRST_READ,
    RTC_ST_WAIT,
    RTC_ST_FAILURE,
};