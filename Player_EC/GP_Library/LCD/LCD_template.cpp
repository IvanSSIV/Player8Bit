 /****************************************************************************/
/* File: LCD_template.cpp                                                   */
/* Data creazione: 26/03/2020  						    */
/* Creato da: Adriano Olivetti - Ivan De Stefani		            */
/* Funzione: Modulo per implementazione gestione display LCD.      	    */
/*									    */
/* Changelog:								    */
/*									    */
/*									    */
/****************************************************************************/
// include comuni in libreria "GP_library"
#include "../GP_library.h"

static BOOL line1_enable;
static BOOL line1_update_req; // richiesta aggiornamento riga 1
static BOOL line2_update_req; // richiesta aggiornamento riga 2
static BOOL line3_update_req; // richiesta aggiornamento riga 3
static BOOL line4_update_req; // richiesta aggiornamento riga 4
static char line1_buffer[LCD_MAX_SIZE + 1];
static char line2_buffer[LCD_MAX_SIZE + 1];

void LCD_hand(void)
  {
     if (line1_update_req)
       {
          #ifdef HW_TP_LCD_MAIN
          dbg_HW_TP0_ON();
          #endif
          line1_update_req = FALSE;
          LCD_write_line1(line1_buffer);
          #ifdef HW_TP_LCD_MAIN
          dbg_HW_TP0_OFF();
          #endif
       }
     if (line2_update_req)
       {
          #ifdef HW_TP_LCD_MAIN
          dbg_HW_TP0_ON();
          #endif
          line2_update_req = FALSE;
          LCD_write_line2(line2_buffer);
          #ifdef HW_TP_LCD_MAIN
          dbg_HW_TP0_OFF();
          #endif
       }

 }


/*-----------------------------------------------------*/
/* Viene richiamata dalla FSM per inviare vari eventi  */
/* al gestore dell'LCD . Il gestore dell'LCD gestisce  */
/* in modo centralizzato gli eventi dell'LCD           */
/*-----------------------------------------------------*/
void LCD_send_event(uint8_t event, char *message)
  {
     switch (event)
       { 
          // cancello tutto il display
          case LCD_EVENT_CLEAR_ALL:
            break;
          // fermo gestione riga 1  
          case LCD_EVENT_LINE1_STOP:
            line1_enable = FALSE;
            break;
          // avvio gestione riga 1
          case LCD_EVENT_LINE1_START:
            line1_enable = TRUE;
            break;
          // richiesta scrittura riga 1
          case LCD_EVENT_UPDATE_LINE1: 
            break;
          // richiesta scrittura riga 2
          case LCD_EVENT_UPDATE_LINE2: 
            line2_update_req = TRUE;
            strncpy(line2_buffer, message, sizeof(line2_buffer) - 1);
            break;
          // richiesta scrittura riga 3         
          case LCD_EVENT_UPDATE_LINE3:
            break;
          // richiesta scrittura riga 4
          case LCD_EVENT_UPDATE_LINE4: 
            break;
          default:
            char buff[50];
            sprintf(buff, "LCD invalid event:%02u", event);
            debug_print_timestamp(lcd_dbg, buff);
       }
  }

/*-----------------------------------------------------------*/
/* Aggiorna le informazioni della riga 1 in modo asincrono   */
/* Viena richiamata dal main della gestione del RTC ad ogni  */
/* variazione dei secondi                                    */
/*-----------------------------------------------------------*/
void LCD_line1_update(char *buffer)
  {
     if (line1_enable)
       { 
          line1_update_req = TRUE;
          strncpy(line1_buffer, buffer, sizeof(line1_buffer)-1);
       }
  }