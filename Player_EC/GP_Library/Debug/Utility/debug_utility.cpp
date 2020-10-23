// include comuni in libreria "GP_library"
#include "..\..\GP_library.h"

// prototipi funzione locali
static void print_timestamp(BOOL ena, DWORD ts);

// prototipi funzione esterni (in Hardware\Debug)
extern void debug_print(char* message);

// variabili locali
static BOOL message_print_ena;
static uint8_t message_ident;


/*-------------------------------------------------------------*/
/* function debug_message_init()                               */
/* Inizializza l'abilitazione e il livello di identazione      */
/* per la visualizzazione dei messaggi con i valori di default */
/*-------------------------------------------------------------*/
void debug_message_init(void)
  {
     message_print_ena = TRUE;
     message_ident = 1;
  }

/*------------------------------------------------------*/
/* function debug_message()                             */
/* Visualizza un messaggio di debug con l'abilitazione  */
/* e il livello di identazione correnti                 */
/*------------------------------------------------------*/
void debug_message(char *message)
  {
     debug_print_ena(message_print_ena,message);
  }

/*------------------------------------------------------*/
/* function debug_message_PGM()                         */
/* Visualizza un messaggio di debug con l'abilitazione  */
/* e il livello di identazione correnti                 */
/*------------------------------------------------------*/
void debug_message_PGM(const char *message)
  {
     debug_print_ena(message_print_ena,AVR_PGM_to_str(message));
  }

/*----------------------------------------------------------*/
/* function debug_message_timestamp()                       */
/* Visualizza un messaggio di debug (completo di timestamp) */
/* con l'abilitazione e il livello di identazione correnti  */
/*----------------------------------------------------------*/
void debug_message_timestamp(char* message)
  {
     debug_print_timestamp_ident(message_print_ena, message_ident, message);
  }

/*----------------------------------------------------------*/
/* function debug_message_timestamp_PGM()                   */
/* Visualizza un messaggio di debug (completo di timestamp) */
/* con l'abilitazione e il livello di identazione correnti  */
/* Come parametro di ingresso accetta una stringa in PGM    */
/*----------------------------------------------------------*/
void debug_message_timestamp_PGM(const char* message)
  {
     if (message_print_ena)
       {
          debug_print_timestamp_ident(TRUE, 
                                      message_ident,
                                      AVR_PGM_to_str(message));
       }
  }
     
/*----------------------------------------------*/
/* function debug_message_CRLF()                */
/* Inserisce una riga di separazione se         */
/* la visualizzazione dei messaggi e' abilitata */
/*----------------------------------------------*/
void debug_message_CRLF(void)
  {
     if (message_print_ena)
       {
          debug_print("\n\r");
       }
  }

/*--------------------------------------------------*/
/* function debug_message_enable()                  */
/* Abilita la visualizzazione dei messaggi di debug */
/* e imposta il livello di identazione corrente     */
/*--------------------------------------------------*/
void debug_message_enable(BOOL ena,uint8_t ident)
  {
     message_ident = ident;
     message_print_ena = ena;
  }

/*-----------------------------------------------------*/
/* function debug_message_enable()                     */
/* Disabilita la visualizzazione dei messaggi di debug */
/*-----------------------------------------------------*/
void debug_message_disable(void)
  {
     message_print_ena = FALSE;
  }

/*-------------------------------------------------*/
/* function debug_message_set_ident()              */
/* Imposta il livello di identazione per la stampa */
/* dei messaggi di debug                           */
/*-------------------------------------------------*/
void debug_message_set_ident(uint8_t ident)
  {
     message_ident = ident;
  }

/*-------------------------------------*/
/* function debug_message_ident_push() */
/* Aumenta il livello di identazione   */
/*-------------------------------------*/
void debug_message_ident_push(void)
  {
     if (message_ident < 8)
       message_ident++;
  }

/*--------------------------------------*/
/* function debug_message_ident_pop()   */
/* Diminuisce il livello di identazione */
/*--------------------------------------*/
void debug_message_ident_pop(void)
  {
     if (message_ident > 0)
       message_ident--;
  }

/*----------------------------------------------------*/
/* function debug_message_tag_ident()                 */
/* Visualizza una stringa (normale) preceduta         */
/* da un tag (in program memory)                      */
/* Prima della tag visulizza il timestamp dell'evento */
/* e eventuali blank per identare                     */
/*----------------------------------------------------*/
void debug_message_tag_ident(const char *tag,const char *message)
  {
  }

/*------------------------------------------------------*/
/* function debug_message_tag_ident_PGM()               */
/* Visualizza una stringa (in program memory) preceduta */
/* da un tag (in program memory)                        */
/* Prima della tag visulizza il timestamp dell'evento   */
/* e eventuali blank per identare                       */
/*------------------------------------------------------*/
void debug_message_tag_ident_PGM(const char *tag,const char *message)
  {
      
  }

//============================================================================//

/*---------------------------------------------------------*/
/* function debug_print_timestamp()                        */
/* Visualizza un messaggio preceduto dal timestamp dei ms  */
/* del Tick di sistema                                     */
/*---------------------------------------------------------*/
void debug_print_timestamp(BOOL ena, char* msg)
{
    print_timestamp(ena, RTOS_get_tick_1ms());
    debug_print_ena(ena, msg);
}
void debug_print_CRLF(BOOL ena)
  {
    if (ena)
      {
         debug_print("\n\r");
      }
  }
void debug_print_timestamp_rel(BOOL ena, char* msg, DWORD ts)
{
    print_timestamp(ena, ts);
    debug_print_ena(ena, msg);
}

//[0000001.115] funzione phNfcLib_GetDataParams() PH_COMP_AC_DISCLOOP
char buff[200], buff1[10];
void debug_print_timestamp_ident(BOOL ena, BYTE ident, char* msg)
{
    BYTE x;

    // vedi se stamp abilitata
    if (!ena)
        return;

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "\n[%010lu] ", (unsigned long int)RTOS_get_tick_1ms());
    buff[14] = 0;
    buff[13] = buff[12];
    buff[12] = buff[11];
    buff[11] = buff[10];
    buff[10] = buff[9];
    buff[9] = '.';

    memset(buff1, 0, sizeof(buff1));
    for (x = 0; x < ident; x++)
        buff1[x] = ' ';
    strcat(buff, buff1);

    // visualizza intenstazione con timestamp
    debug_print(buff);

    // visualizza messaggio vero e proprio
    debug_print(msg);
}

void debug_printbuff_hex(BOOL ena, BYTE* buff, BYTE buff_size)
{
    BYTE x;
    char temp[50];

    if (sizeof(temp) <= (3 * buff_size))
    {
        debug_print_ena(ena, (char*)"********");
        return;
    }
    for (x = 0; x < buff_size; x++)
    {
        sprintf(temp, "%02X", buff[x]);
        if (x == buff_size - 1)
            strcat(temp, " ");
        else
            strcat(temp, "-");
        debug_print_ena(ena, temp);
    }
}

void debug_make_buff_hex(char* buff, uint8_t size, uint8_t* data, uint8_t data_size)
{
    uint8_t x, buff_request;
    char temp[4];

    buff_request = (3 * data_size) + 2;

    // verifica che il buffer di stampa sia sufficente per i caratteri da inserire
    if (size > buff_request)
    {
        strncpy(buff, "[", size - 1);
        for (x = 0; x < data_size; x++)
        {
            sprintf(temp, "%02X", data[x]);
            if (x < data_size - 1)
                strcat(temp, "-");
            strncat(buff, temp, size - 1);
        }
        strncat(buff, "]", size - 1);
    }
    buff[buff_request - 1] = 0;
}
//================================================================================//
static void print_timestamp(BOOL ena, DWORD ts)
{
    char buff[30];

    sprintf(buff, "\n[%010lu] ", ts);
    buff[15] = 0;
    buff[14] = buff[13];
    buff[13] = buff[12];
    buff[12] = buff[11];
    buff[11] = buff[10];
    buff[10] = buff[9];
    buff[9] = '.';
    debug_print_ena(ena, buff);

}

void debug_print_ena(BOOL ena, char* message)
  {
     if (ena)
       {
          debug_print(message);
       }
  }

//===========================================================================//
void debug_print_timestamp_title(BOOL ena,uint8_t ident, char* title, char* msg)
{
    debug_print_timestamp_ident(ena,ident,title);
    debug_print_ena(ena, msg);
}
