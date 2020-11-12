/****************************************************************************/
/* File: appl_main.cpp                                                      */
/* Data creazione: 17/03/2020                                               */
/* Creato da: Adriano Olivetti - Ivan De Stefani                            */
/* Funzione: Main del codice di gestione macchina.                          */
/*                                                                          */
/* Changelog:                                                               */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
#include "player_EC.h"

//=======================================================================================//
// stringhe con "nome_apparato" e "versione_firmware" per
// messaggio di boot su seriale di debug
static const char str_nome_apparato[34] PROGMEM = { "\n*          Player-EC           *" };
static const char str_versione_FW[34] PROGMEM   = { "\n*    V 0.90.24 (11-11-2020)    *" };

const char *get_nome_apparato(void)
  {
     return str_nome_apparato;
  }

const char *get_versione_FW(void)
  {
     return str_versione_FW;
  }
//========================================================================================//

void appl_main(void)
  {
     static uint8_t print_request=0;

     // gestione comandi ESC da seriale
     serial_ESC_command();

     // gestione base tempi a multipli di 1mS
     basetempi_RTOS_main();

  
     // vedi se basetempi a 10ms
     if (flg_bt_10ms)
       {
          // gestione PLC + logica
          // appl_test_PLC_main();
          logic_main();
          
          // Gestione motore ad alto livello
          motor_hand();

          // Gestione lettura badge su Serial2
          badge_hand();

          // Gestione erogazione ad alto livello
          erog_hand();
             
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
            //hi_speed_print();
       }
     #ifdef DEBUG_HARDWARE
     if (print_request == 2)
       {
         print_request = 0;

         dbg_HW_TP1_ON();
        
         PLC_print_state(io_dbg, &PLC_IO_data);

         dbg_HW_TP1_OFF();
       }
     #endif
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
     server_request_hand();

     // gestione servizi memorizzazion locali
     storage_hand();

    

     // azzera flag delle basetempi
     basetempi_RTOS_clear_flg();
  }


/*--------------------------------------------*/
/* function appl_main_PLC_read_input()*/
static void appl_main_PLC_read_input(void)
  {
     // lettura ingressi
     #ifdef HW_TP_PLC_RW
     dbg_HW_TP2_ON();
     #endif
     PLC_read_input(&PLC_IO_data);         // 10 us
     #ifdef HW_TP_PLC_RW
     dbg_HW_TP2_OFF();
     #endif
  }