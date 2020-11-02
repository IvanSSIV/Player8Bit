
// Il modulo RTC_driver_DS1307.cpp si occupa di leggere e scrivere l'ora 
// dal chip DS1307 ed e' quindi la parte strettamente hardware dipendente.
// Si appoggia per questa attivita sulla classe DS1307RTC delle libreirie Arduino
// È costituito da un funzione di inizializzazione che inizializza il chip
// e ne verifica l'esistenza e il suo funzionamento . Verifica inoltre la 
// correttezza delle informazioni (es: batteria scarica)
// A livello di applicazione ha una funzione "RTC_gettime_low_level" .
// Questa funzione legge in polling la data dal RTC e attende il cambio dei secondi
// Al verificarsi di questa condizione notifica al main l'evento avvenuto e aggiorna
// una struttura globale con le infornazioni di data-ora attuali.
// Per minimizzare l'uso della risorsa "I2C" il driver attende circa 980ms dopo
// ogni variazione per evitare inutili letture.
// La gestione del RTC a alto livello si appoggia al driver e legge la dataora 
// da questa struttura globale , mediante una chiamata alla "RTC_gettime_hl"
// Include di librerie
#include "stdint.h"
#include "string.h"
#include <Wire.h>
#include "Arduino.h"
#include <DS1307RTC/DS1307RTC.h>

#include "debug_opzion.h"
#include "My_Include/mydef.h"
#include "My_Include/my_time.h"
#include "My_Library/Utilities/AVR_API.h"

#include "Hardware/CPU_core/timing.h"

#include "My_Debug/Debug_Utility/debug_utility.h"
#include "My_Debug/Debug_HW/debug_HW.h"
#include "My_Debug/Serial_Command/serial_command.h"
#include "gestione_RTC.h"

// Stringhe costanti per messaggi di debug su seriale
const char str_dbg_RTC_init_start[] PROGMEM = { "inizializzo RTC HW : " };
const char str_dbg_RTC_init_datetime_KO[] PROGMEM = { "data-ora corrotti" };
const char str_dbg_RTC_init_chip_KO[] PROGMEM = { "chip failure" };
const char str_dbg_RTC_init_end[] PROGMEM = { "fine inizializzazione RTC HW ..." };
const char str_dbg_RTC_update_start[] PROGMEM = { "Imposto il modulo RTC con domanda al server..." };
const char str_dbg_RTC_update_end[] PROGMEM = { "Fine aggiornamento RTC su PCtime!" };

// definizioni locali
DS1307RTC RTCLanzi;		       // Elemento della classe DS1307

// variabili locali
static RTC_WORK rtc_work;      // struttura con dati RTC
static BOOL     RTC_failure;   // Flag per failure HW o logico dell'RTC
static BOOL     RTC_dataloss;  // perdita dtai RTC
stato_RTC       RTC_read_stat; // Stato per la lettura dell'RTC a livello di main

// prototipi funzione locali
static BOOL RTC_get_datetime_LL(RTC_WORK* my_tm);
void RTC_disalign_LL(void);
/*********************************************************/
/* void RTC_init(void)                                   */
/*_______________________________________________________*/
/* Inizializzazione del chip DS1307 con failure control. */
/*                                                       */
/* Parametri: nessuno                                    */
/*********************************************************/

void RTC_init_LL(void)
{
    tmElements_t tm;

    debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_RTC_init_start));
    RTC_failure = FALSE;
    RTC_dataloss = FALSE;
    // la "read" restituisce TRUE se la lettura e' andata a buon fine
    // oppure FALSE se il chip non risponde o il bit di oscillatore segnala
    // oscillatore spento
    if (RTCLanzi.read(tm))
    {
        debug_print_ena(DBG_ALWAYS_ON, "ok");
        // lettura OK
        RTC_get_datetime_LL(&rtc_work);
        /*
                  rtc_work.hour = 12;
                  rtc_work.min = 24;
                  rtc_work.sec = 36;*/
    }
    else
    {
        // guasto HW o oscillatore spento (batteria scarica o assente)
        // la "chipPresent" restituisce TRUE se il chip lavora
        // e di conseguenza consente di discriminare tra chip guasto e batteria
        if (RTCLanzi.chipPresent())
        {
            // passa di qui se Data e ora corrotti
            RTC_dataloss = TRUE;
            debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_RTC_init_datetime_KO));  
            RTC_disalign_LL();
        }
        else
        {
            // passa di qui Chip failure
            RTC_failure = TRUE;
            debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_RTC_init_chip_KO));     
        }
    }

    //RTC_print(&rtc_work);
    RTC_read_stat = RTC_ST_INIT;
    debug_print_timestamp(DBG_ALWAYS_ON, AVR_PGM_to_str(str_dbg_RTC_init_end));                    // Fine inizializzazione RTC hardware
}


/***************************************/
/* void RTC_hand(void)                 */
/*_____________________________________*/
/* Lettura dell'RTC a livello di main. */
/*                                     */
/* Parametri: nessuno                  */
/***************************************/
/* function RTC_read_gettime_low_level()                  */
/* Legge la data e l'ora correnti a basso livello ovvero  */
/* dal RTC e la memorizza  n una struttura globale        */
/* per il programma applicativo                           */
/* Restiuisce TRUE a ogni cambio del secondo              */

BOOL RTC_gettime_low_level(void)
{
    static uint8_t last_sec;
    static uint32_t local_millis;
    //static char rtcbuff[25];
    BOOL retval;

    retval = FALSE;
    switch (RTC_read_stat)
    {
    case RTC_ST_INIT:
        last_sec = 0xFF;
        //RTC_update(&rtc_work, 606998124L);     // da implementare dopo la richiesta PCtime
        RTC_read_stat = RTC_ST_FIRST_READ;
        break;

    case RTC_ST_FIRST_READ:
        BOOL esito;
        #ifdef HW_TP_RTC_MAIN
        dbg_HW_TP1_ON();
         #endif
        // impiega circa xx ms
        esito = RTC_get_datetime_LL(&rtc_work);
        #ifdef HW_TP_RTC_MAIN
        dbg_HW_TP1_OFF();
        #endif
        if (esito)
        {
            if (rtc_work.sec != last_sec)
            {
                last_sec = rtc_work.sec;
                local_millis = RTOS_get_tick_1ms();
                RTC_read_stat = RTC_ST_WAIT;
                retval = TRUE;
            }
        }
        else
        {
            // qui inserire diagnostica potenziale malfunzionamento del RTC
        }
        break;

    case RTC_ST_WAIT:
        if ((RTOS_get_tick_1ms() - local_millis) > 980L)
        {
            RTC_read_stat = RTC_ST_FIRST_READ;
        }
        break;

    case RTC_ST_FAILURE:
        break;
    default:
        break;
    }

    return retval;
}

/**************************************************************************/
/* void RTC_get_datetime(RTC_WORK* my_tm)                                 */
/*________________________________________________________________________*/
/* Inserisce nella struttura passata come parametro la lettura dei dati   */
/* dall'RTC DS1307.                                                       */
/*                                                                        */
/* Parametri: RTC_WORK * my_tm : puntatore alla struttura RTC da popolare */
/**************************************************************************/

static BOOL RTC_get_datetime_LL(RTC_WORK* my_tm)
{
    tmElements_t tm;

    // la "read" restituisce TRUE se il chip lavora a livello di I2C
    // e l'oscillatore non e' fermo
    if (RTCLanzi.read(tm))
    {
        my_tm->hour = tm.Hour;
        my_tm->min = tm.Minute;
        my_tm->sec = tm.Second;
        my_tm->day = tm.Day;
        my_tm->month = tm.Month;
        my_tm->year = tm.Year;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



/* Legge e passa all'applicazione ad alto livello la data e l'ora letti dal RTC*/
/* va chiamata immediatamente dopo che la "RTC_gettime_low_level" ha restituito TRUE */
void RTC_gettime_HL(RTC_WORK* time_buff)
{
    time_buff->hour = rtc_work.hour;
    time_buff->min = rtc_work.min;
    time_buff->sec = rtc_work.sec;
    time_buff->dow = rtc_work.dow;
    time_buff->day = rtc_work.day;
    time_buff->month = rtc_work.month;
    time_buff->year = rtc_work.year + 1970;
}

void RTC_settime_LL(RTC_WORK* time_buff)
{
    tmElements_t loc_time;
    
    loc_time.Hour   = time_buff->hour;
    loc_time.Minute = time_buff->min;
    loc_time.Second = time_buff->sec;
    loc_time.Wday   = time_buff->dow;
    loc_time.Day    = time_buff->day;
    loc_time.Month  = time_buff->month;
    loc_time.Year   = time_buff->year-1970;
    /*
    char buff[80];
    sprintf(buff, "RTC_settime_LL :%02u-%02u-%04u", loc_time.Day, loc_time.Month, loc_time.Year);
    debug_print_timestamp(DBG_ALWAYS_ON, buff);
    */
    RTCLanzi.write(loc_time);
}


void RTC_disalign_LL(void)
{
    RTC_WORK loc_time;
    memset(&loc_time, 0, sizeof(loc_time));
    loc_time.day = 1;
    loc_time.month = 1;
    loc_time.year = 2020;
    RTC_settime_LL(&loc_time);
}
