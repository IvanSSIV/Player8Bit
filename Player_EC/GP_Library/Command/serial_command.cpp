#define MODULE_SERIAL_COMMAND

// include comuni in libreria "GP_library"
#include "..\GP_library.h"
#include "Application_Code/Test/serial_command_test.h"
// definizione variabili locali
static ESC_HAND_STATE ESC_stat;        // stato sequenza di ESC
static uint8_t        ESC_first_char;  // primo carattere dopo ESC
static uint8_t        ESC_second_char; // secondo carattere dopo ESC
static uint32_t       ESC_timeout;     // timeout chiusura sequenza


// prototipi funzione locali
static uint8_t serial_ESC_read_char(void);



/**********************************************/
/* function serial_ESC_command()              */
/* gestisce la ricezione di seguenze di  */
/* sulla seriale MPC2200                      */
/**********************************************/
void serial_ESC_command(void)
  {
     char c;
     BOOL done;
     switch (ESC_stat)
       {
          case ST_ESC_WAIT:            
            c = serial_ESC_read_char();
            // vedi se arrivato ESC
            if (c == 0x1B)
              {
                 ESC_timeout = RTOS_get_tick_1ms();
                 ESC_stat = ST_ESC_FIRST;
              }
            break;

          case ST_ESC_FIRST:
            c = serial_ESC_read_char();
            if (c != 0)
              {
                 switch (c)
                   {              
                      case 'P': // comandi specifici player  
                      case 'D': // debug command
                      case 'T': // comandi test
                      case 'L': // coamndi libreria LANZI
                      case 'S': // comandi di sistema
                      case 'H': // comandi hardware
                        ESC_first_char = c;
                        ESC_stat = ST_ESC_SECOND;
                        break;
                      default:
                        ESC_stat = ST_ESC_WAIT;
                   }
              }
            break;

          case ST_ESC_SECOND:
            c = serial_ESC_read_char();
            if (c != 0)
              {
                 switch (ESC_first_char)
                   {
                      case 'P': // comandi specifici player
                        done = serial_ESC_player(c);
                        break;
                      case 'D': // debug command
                        done = serial_ESC_debug(c);
                        break;
                      case 'T': // comandi test
                        done = serial_ESC_test(c);  
                        break;
                      case 'L': // library command
                        done = serial_ESC_LANZI(c);
                        break;
                      case 'S': // impostazioni di sistema
                        done = serial_ESC_system(c);
                        break;
                    }
                 if (done)
                   {
                      ESC_stat = ST_ESC_WAIT;
                   }
              }
            break;
       }

     // gestione timeout chiusura sequenza
     if (ESC_stat != ST_ESC_WAIT)
       {
          if ((RTOS_get_tick_1ms() - ESC_timeout) > 3000L)
            {
               ESC_stat = ST_ESC_WAIT;
            }
       }
  }




BOOL serial_ESC_system(char c)
  {
     char buff[80];
     sprintf(buff,"ricevuto comando <ESC>S%c",c);
     debug_print_timestamp(TRUE,buff);
     return TRUE;
  }

/*---------------------------------------------------------*/
/* function seriaL_ESC_read_char()                         */
/* Attende l'arrivo di un carattere dalla seriale di debug */
/* In uscita restituisce il carattere ricevuto o 0x00 se   */
/* non vi e' nulla . Questa funzione prevede come ingresso */
/* un flusso di caratteri ASCII dove non sia mai presente  */
/* il carattere 0x00                                       */
/*---------------------------------------------------------*/
static uint8_t serial_ESC_read_char(void)
  {
     uint8_t retval;

     retval = 0;
     if (debug_char_available())
       {
          retval = debug_read_char();
          retval = toupper(retval);
       }
     return retval;
  }


#undef MODULE_SERIAL_COMMAND