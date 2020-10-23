// include comune librerie LANZI
#include "../Lanzi_library.h"

// prototipi funzione locali
static void print_FSM_storage_state(BOOL ena,uint8_t stato,char *str_out,uint8_t str_size);

/*-------------------------------------------------------*/
/* function debug_print_FSM_storage                      */
/* Visualizza una serie di informazioni della macchina   */
/* a stati gestione dello storage su SD .                */
/* - timestamp ai ms dell'evento                         */
/* - stato attuale della macchina a stati                */
/* - messaggio vero e proprio                            */
/*-------------------------------------------------------*/
void debug_print_FSM_storage(STORAGE_WORK *ptr,char *msg)
  {
     uint8_t stato;
     char stato_str[20];
  
     // vedi se debug globale abilitato
     if (!ptr->debug_ena)
       return;
  
     // seleziona lo stato attuale
     stato = ptr->stat;
     // prepara il nome dello stato
     print_FSM_storage_state(TRUE,stato,stato_str,sizeof(stato_str));
     // visualizza timestamp
     debug_print_timestamp_ident(TRUE,ptr->debug_ident,stato_str);
     // visualizza messaggio vero e proprio
     debug_print_ena(TRUE,msg);
  }

/*---------------------------------------------*/
/* function print_FSM_storage_state()          */
/* visualizza il nome dello stato corrente     */
/*---------------------------------------------*/
static void print_FSM_storage_state(BOOL ena,uint8_t stato,char *str_out,uint8_t str_size)
  {
     char buff[30];
     
     strcpy(buff,"[STO:");
     switch (stato)
       {
          case ST_STORAGE_INIT:
            strcat(buff,"INIT       ");
            break;
          // impostazione locali
          case ST_STORAGE_WAIT_CMD:
            strcat(buff,"WAIT_CMD   ");
            break;
          // verifica file "apertura.dat"
          case ST_STORAGE_CHK_APERTURA:
            strcat(buff,"CHECK_APER ");
            break;
          // verifica file "chiusura.dat"
          case ST_STORAGE_CHK_CHIUSURA:
            strcat(buff,"CHECK_CHIUS");
            break;
          // verifica file "ricarica.dat"
          case ST_STORAGE_CHK_RICARICA:
            strcat(buff,"CHECK_RICAR");
            break;
          // legge file "ricarica.dat"
          case ST_STORAGE_READ_RICARICA:
            strcat(buff,"READ_RICAR ");
            break;
          // legge abilitazioni 
          case ST_STORAGE_READ_ABIL:
            strcat(buff,"READ_ABIL  ");
            break;
          // avvenuto errore
          case ST_STORAGE_ERROR:
            strcat(buff,"ERROR      ");
            break;
          default:
            strcat(buff,"?????????? ");
       }
     strcat(buff,"] ");
     strncpy(str_out,buff,str_size);
  }
