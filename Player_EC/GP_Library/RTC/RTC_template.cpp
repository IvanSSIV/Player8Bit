/******************************************************************************/
/* File: RTC_Template.cpp                                                     */
/* Data creazione: 26/03/2020  												  */
/* Creato da: Ivan De Stefani  												  */
/* Funzione: creazione di una struttura modulare per la gestione dei moduli   */
/*			 Real Time Clock; nel caso Lanzi, DS1307.						  */
/* Changelog:																  */
/*																			  */
/*	27/3/2020   IDS      Aggiunta funzione per aggiornamento RTC successivo   */
/*                       alla richiesta del PCtime al server; aggiunta stampa */
/*                       su LCD dei valori letti dal DS1307.                  */
/******************************************************************************/
// include comuni in libreria "GP_library"
#include "../GP_library.h"

// data ora correnti in ASCII (formato LANZI)
static char data_ora_ASCII_LANZI[9];

// data ora correnti in ASCII (formato standard)
static char data_ora_ASCII[30];

// data ora correnti in ASCII (formato file dat)
static char data_ora_ASCII_filedat[30];

static BOOL ena_rtc_print;

// tabella conversione da numero a carattere ASCII
static const uint8_t ascii_char_tab[36] =
{
    // [00-07] 0,1,2,3,4,5,6,7
    48,49,50,51,52,53,54,55,
    // [08-15] 8,9,A,B,C,D,E,F
    56,57,65,66,67,68,69,70,
    // [16-23] G,H,I,J,K,L,M,H
    71,72,73,74,75,76,77,78,
    // [24-31] O,P,Q,R,S,T,U,V
    79,80,81,82,83,84,85,86,
    // [32-36] W,X,Y,Z
    87,88,89,90
};

// Prototipi funzione locali
static void datetime_to_ASCII_LANZI(RTC_WORK* dataora, char* dataora_ASCII);
static void convert_datetime_long_to_HMSDMY(DWORD t, RTC_WORK* dt_out);
static BOOL convert_ASCII_to_DWORD(char* dato_ASCII, DWORD* dato_DW);

/*----------------------------------------------------------------------*/
/* function leggi_data_ora_da_RTC()                                     */
/* Legge e memorizza in una variabile globale la data e l'ora correnti  */
/* Viene richiamata dal main loop ad ogni cambiamento del secondo       */
/*----------------------------------------------------------------------*/
void leggi_data_ora_da_RTC(void)
{
    char buff[120];

    RTC_WORK data_ora_RTC;

    // acquisisci data e ora letti dal driver di basso livelli
    RTC_gettime_HL(&data_ora_RTC);

    // prepara stringa per filedat
    sprintf(data_ora_ASCII_filedat, "%02d/%02d/%04d  %02d:%02d:%02d",
        data_ora_RTC.day, data_ora_RTC.month, data_ora_RTC.year,
        data_ora_RTC.hour, data_ora_RTC.min, data_ora_RTC.sec);

    // prepara string ASCII classico
    sprintf(data_ora_ASCII, "%02d:%02d:%02d %02d-%02d-%04d",
        data_ora_RTC.hour, data_ora_RTC.min, data_ora_RTC.sec,
        data_ora_RTC.day, data_ora_RTC.month, data_ora_RTC.year);

    // prepara string formato LANI
    datetime_to_ASCII_LANZI(&data_ora_RTC, data_ora_ASCII_LANZI);

    // visualizzali sulla seriale
    if (ena_rtc_print)
    {
        sprintf(buff, "dataora RTC  classic=%s  LANZI=%s", data_ora_ASCII, data_ora_ASCII_LANZI);
        debug_print_timestamp(TRUE, buff);
    }
}

/*------------------------------------------*/
/* Abilita visualizzazione data-ora del RTC */
/*------------------------------------------*/
void RTC_ena_print(void)
{
    ena_rtc_print = TRUE;
}

/*---------------------------------------------*/
/* Disabilita visualizzazione data-ora del RTC */
/*---------------------------------------------*/
void RTC_dis_print(void)
{
    ena_rtc_print = FALSE;
}

/*-----------------------------------------------------------*/
/* function get_datetime_ASCII_LANZI()                       */
/* Legge e la data-ora corrente nel formato ASCII (standard) */
/*-----------------------------------------------------------*/
void get_datetime_ASCII(char* date_time, uint8_t date_size)
{
    strncpy(date_time, data_ora_ASCII, date_size);
}

/*-------------------------------------------------------------------*/
/* function get_datetime_ASCII_LANZI_filedat()                       */
/* Legge e la data-ora corrente nel formato ASCII (formato file dat) */
/*-------------------------------------------------------------------*/
void get_datetime_ASCII_filedat(char* date_time, uint8_t date_size)
{
    strncpy(date_time, data_ora_ASCII_filedat, date_size);
}

/*-----------------------------------------------------------*/
/* function get_datetime_ASCII_Lanzi()                       */
/* Legge e la data-ora corrente nel formato ASCII (LANZI)    */
/*-----------------------------------------------------------*/
void get_datetime_ASCII_Lanzi(char* date_time, uint8_t date_size)
{
    strncpy(date_time, data_ora_ASCII_LANZI, date_size);
    date_time[date_size] = 0;
}

/*------------------------------------------------------------*/
/* Imposta l'ora del RTC locale con l'informazione            */
/* ricevuta dal server tramite il servizio PCtime             */
/* "data_ora_server" e' una stringa di caratterri ASCII  come */
/* ricevuta dal server , ripulita da preamboli vari           */
/* restituisce TRUE se i parametri in ingresso sono corretti  */
/*------------------------------------------------------------*/
BOOL set_data_ora_da_server(char* data_ora_server)
{
    DWORD loc_time;
    RTC_WORK tmp_time;

    // converti da secondi a struttura RTC_WORK
    if (convert_ASCII_to_DWORD(data_ora_server, &loc_time))
      {
          convert_datetime_long_to_HMSDMY(loc_time, &tmp_time);
          // chiama driver per impostare l'ora
          RTC_settime_LL(&tmp_time);
          RTC_print(&tmp_time);
          return TRUE;
      }
    else
      {
         return FALSE;
      }
}

static BOOL convert_ASCII_to_DWORD(char* dato_ASCII, DWORD* dato_DW)
{
    uint8_t j, size;
    DWORD temp = 0;
    char c;

    size = strlen(dato_ASCII);
    // estrai i dati dal buffer
    for (j = 0; j < size; j++)
    {
        c = dato_ASCII[j];
        if (isdigit(c))
            temp = temp * 10 - 48 + c;
        else
            return FALSE;
    }
    *dato_DW = temp;
    return TRUE;
}
//=========================================================================//

/*----------------------------------------------------------------*/
/* converte da un formato ora (sever LANZI) a struttura RTC_WORK  */
/*----------------------------------------------------------------*/
static void convert_datetime_long_to_HMSDMY(DWORD t, RTC_WORK *dt_out)
{
    int gg_per_mese[12];

    long t_ora = t % 86400;
    t = t - t_ora;

    int secondi = t_ora % 60;
    t_ora = (t_ora - secondi) / 60;

    int minuti = t_ora % 60;
    t_ora = (t_ora - minuti) / 60;

    int ore = t_ora;
    int t_giorno = t / 86400;
    int new_anno = 2001;
    int gg_anno = 0;
    int anno = 0;
    int new_giorno = t_giorno;
    while (new_giorno >= 0)
    {
        gg_anno = new_giorno; anno = new_anno;
        new_anno = new_anno + 1; new_giorno = new_giorno - 365;
        if ((new_anno % 4) == 0 && new_giorno > 365)
        {
            new_giorno = new_giorno - 1;
        }
    }

    int mese = 0;
    int giorno = 0;
    if ((anno % 4) == 0)
    {
        gg_per_mese[0] = 0, gg_per_mese[1] = 31; gg_per_mese[2] = 60; gg_per_mese[3] = 91; gg_per_mese[4] = 121;
        gg_per_mese[5] = 152; gg_per_mese[6] = 182; gg_per_mese[7] = 213; gg_per_mese[8] = 244; gg_per_mese[9] = 274;
        gg_per_mese[10] = 305; gg_per_mese[11] = 335;
    }
    else
    {
        gg_per_mese[0] = 0, gg_per_mese[1] = 31; gg_per_mese[2] = 59; gg_per_mese[3] = 90; gg_per_mese[4] = 120;
        gg_per_mese[5] = 151; gg_per_mese[6] = 181; gg_per_mese[7] = 212; gg_per_mese[8] = 243; gg_per_mese[9] = 273;
        gg_per_mese[10] = 304; gg_per_mese[11] = 334;
    }

    for (int i = 0; i <= 11; i++)
    {
        if (gg_anno >= gg_per_mese[i])
        {
            mese = i + 1;
            giorno = gg_anno - gg_per_mese[i] + 1;
        }
    }

    dt_out->hour = ore;
    dt_out->min = minuti;
    dt_out->sec = secondi;
    dt_out->dow = 4;
    dt_out->day = giorno;
    dt_out->month = mese;
    dt_out->year = anno;

}


/*----------------------------------------------------------------*/
/* function datetime_to_ASCII_LANZI()                             */
/* Converte la data-ora nel formato ASCII usato dal server LANZI  */
/*----------------------------------------------------------------*/
static void datetime_to_ASCII_LANZI(RTC_WORK* dataora, char* dataora_ASCII)
{
    char tmp_buff[10];

    // prepara minuti e secondi   
    sprintf(tmp_buff, "%02d%02d", dataora->min, dataora->sec);

    // anno (a due cifre)
    uint8_t index = (uint8_t)(dataora->year - 2000);
    index =  (index > 35) ? 35 : index;
    dataora_ASCII[0] = ascii_char_tab[index];
    // mese
    dataora_ASCII[1] = ascii_char_tab[dataora->month];
    // giorno mese
    dataora_ASCII[2] = ascii_char_tab[dataora->dow];
    // ore
    dataora_ASCII[3] = ascii_char_tab[dataora->hour];
    // minuti 
    dataora_ASCII[4] = tmp_buff[0];
    dataora_ASCII[5] = tmp_buff[1];
    // secondi
    dataora_ASCII[6] = tmp_buff[2];
    dataora_ASCII[7] = tmp_buff[3];

    dataora_ASCII[8] = 0;
}


/****************************************************************/
/* void RTC_print(RTC_WORK* dt)                                 */
/*______________________________________________________________*/
/* Stampa su seriale di debug dei dati dell'RTC.                */
/*                                                              */
/* Parametri: RTC_WORK* dt: puntatore alla struttura da leggere */
/****************************************************************/

void RTC_print(RTC_WORK* dataora)
{
    char buff[100], temp[10];

    datetime_to_ASCII_LANZI(dataora, temp);

    sprintf(buff, " %02d:%02d:%02d %02d-%02d-%04d %s",
        dataora->hour, dataora->min, dataora->sec, dataora->day, dataora->month, dataora->year, temp);
    debug_print_ena(TRUE, buff);
    debug_print_timestamp(rtc_dbg, buff);
}


