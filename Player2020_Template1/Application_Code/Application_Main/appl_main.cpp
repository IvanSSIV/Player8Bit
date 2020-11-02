/****************************************************************************/
/* File: appl_main.cpp                                                      */
/* Data creazione: 17/03/2020 												*/
/* Creato da: Adriano Olivetti - Ivan De Stefani							*/
/* Funzione: Main del codice di gestione macchina.                 			*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

#include "player_header.h"

void appl_main(void)
{
     static uint8_t print_request=0;
     serial_get_command();

     // gestione base tempi a multipli di 1mS
     basetempi_RTOS_main();

     // vedi se basetempi a 10ms
     if (flg_bt_10ms)
       {
          // gestione PLC + logica
          appl_test_PLC_main();
          
          // Gestione motore ad alto livello
          motor_hand();

          if (print_request == 1)
              print_request = 2;
       }

     // vedi se bastempi a 100ms
     if (flg_bt_100ms)
       {
          // gestione LCD ad alto livello
          LCD_hand();
       }
     

     // vedi se basetempi a 1sec
     if (flg_bt_1s)
       {
            print_request = 1;
       }

     if (print_request == 2)
       {
         print_request = 0;
         dbg_HW_TP1_ON();
         PLC_print_state(io_dbg, &PLC_work);
         dbg_HW_TP1_OFF();
     }
       
     /*if (PLC_Input_changed())
       {

       }
     */
     // lettura RTC
     // nb: restituisce TRUE una volta al secondo
     if (RTC_gettime_low_level())
       {
          static uint8_t count = 0;
          // getsisce la parte dell'ora hardware indipentente
          leggi_data_ora_da_RTC();

          if (count < 20)
            {
               count++;
               if (count == 20)
                 {
                   RTC_dis_print();
                 }
            }

       }

     // Gestione comunicazione con il server
     Server_request_hand();

     // azzera flag delle basetempi
     basetempi_RTOS_clear_flg();
  }
