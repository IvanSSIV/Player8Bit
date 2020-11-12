#define MODULE_SERIAL_COMMAND

// include comuni in libreria "GP_library"
#include "../GP_library.h"
#include "../../Application_Code/Test/serial_command_test.h"
#include "../../Lanzi_Library/Command/serial_command_LANZI.h"
#include "../../Lanzi_Library/Include/lanzi_def.h"

// definizione variabili locali
static ESC_HAND_STATE ESC_stat;        // stato sequenza di ESC
static uint8_t        ESC_first_char;   // primo carattere dopo ESC
static uint32_t       ESC_timeout;     // timeout chiusura sequenza
static uint8_t        ESC_sub_stat;
static char           ESC_buff[16];


// prototipi funzione locali
static uint8_t serial_ESC_read_char(void);
static BOOL serial_ESC_player_top(char c);


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
                 ESC_stat = ST_ESC_CH1;
              }
            break;

          case ST_ESC_CH1:
            c = serial_ESC_read_char();
            if (c != 0)
              {
                 //debug_message_timestamp("ESC_st1");
                 switch (c)
                   {              
                      case 'P': // comandi specifici player  
                      case 'D': // debug command
                      case 'T': // comandi test
                      case 'L': // comandi libreria LANZI
                      case 'S': // comandi di sistema
                      case 'H': // comandi hardware
                        ESC_first_char = c;
                        ESC_sub_stat = 0;
                        ESC_stat = ST_ESC_CH2;
                        break;
                      default:
                        ESC_stat = ST_ESC_WAIT;
                   }
              }
            break;

          case ST_ESC_CH2:
            c = serial_ESC_read_char();
            if (c != 0)
              {
                 //char buff[80];
                 //sprintf(buff, "ESC_st2 c=%02x-%c prv=%02x-%c",c,c,ESC_first_char,ESC_first_char);
                 //debug_message_timestamp(buff);
                 switch (ESC_first_char)
                   {
                      case 'P': // comandi specifici player
                        done = serial_ESC_player_top(c);
                        break;
                      case 'D': // debug command
                        done = serial_ESC_debug(c);
                        break;
                      case 'T': // comandi test
                        done = serial_ESC_test(c);  
                        break;
                      case 'L': // library Lanzi
                        done = serial_ESC_LANZI(c,&ESC_stat);
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

          case ST_ESC_CH3:
            c = serial_ESC_read_char();
            if (c != 0)
              {
                 //char buff[80];
                 //sprintf(buff, "ESC_st3 c=%02x-%c prv=%02x-%c",c,c,ESC_first_char,ESC_first_char);
                 //debug_message_timestamp(buff);
                 switch (ESC_first_char)
                   {
                      case 'L':  // library Lanzi
                        done = serial_ESC_LANZI(c,&ESC_stat); 
                        break;
                   }
                 if (done)
                   {
                      ESC_stat = ST_ESC_WAIT;
                   }
              }
            break;

          case ST_ESC_CH4:
            break;

       }

     // gestione timeout chiusura sequenza
     if (ESC_stat != ST_ESC_WAIT)
       {
          if ((RTOS_get_tick_1ms() - ESC_timeout) > 3000L)
            {
               debug_message_timestamp("azzero sequenza di ESC");
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
          //char buff[80];
          //sprintf(buff,"ricevuto carattere : %02x",retval);
          //debug_message_timestamp(buff);
       }
     return retval;
  }


static BOOL serial_ESC_player_top(char c)
  {
     static uint8_t player_type;
     BOOL retval;

     retval = FALSE;
     switch (ESC_sub_stat)
       {
          case 0:
            memset(ESC_buff,0,sizeof(ESC_buff));
            ESC_buff[0] = c;
            ESC_sub_stat++;
            break;
          case 1:
            ESC_buff[1] = c;
            ESC_sub_stat++;
            if (strncmp(ESC_buff,"EC",2) == 0)
              {
                 player_type = ARGO_EC;
              }
            else if (strncmp(ESC_buff,"KD",2) == 0)
              {
                 player_type = ARGO_KD;
              }
            else if (strncmp(ESC_buff,"LK",2) == 0)
              {
                 player_type = ARGO_LK;
              }
            else if (strncmp(ESC_buff,"GR",2) == 0)
              {
                 player_type = ARGO_GR;
              }
            else if (strncmp(ESC_buff,"XL",2) == 0)
              {
                 player_type = ARGO_XL;
              }
            break;

          case 2:
            retval = serial_ESC_player(c);
            break;
       }

     return retval;
  }
#undef MODULE_SERIAL_COMMAND