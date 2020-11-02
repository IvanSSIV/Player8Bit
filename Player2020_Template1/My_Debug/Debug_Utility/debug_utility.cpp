#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "My_Include/mydef.h"
#include "Hardware/CPU_core/timing.h"
#include "My_Debug/Debug_Serial/debug_serial.h"
#include "debug_utility.h"
#include "My_Library/Utilities/AVR_API.h"

// prototipi funzione locali
static void print_timestamp(BOOL ena, DWORD ts);

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
        debug_print(message);
}

//===========================================================================//
void debug_print_timestamp_title(BOOL ena,uint8_t ident, char* title, char* msg)
{
    debug_print_timestamp_ident(ena,ident,title);
    debug_print_ena(ena, msg);
}
