/*--------------------------------------------------------------------*/
/* Progetto  : programma gestione Player-DA su scheda 32bit           */
/* Modulo    : Modulo gestione memorizzazioni esterne                 */
/* Data      : 06-12-2018                                             */
/* Nome file : player_storage.cpp                                     */
/* Operatore : Adriano Olivetti                                       */
/* Note      :                                                        */
/* Gestisce le operazioni connesse alla memorizzazione di dati su     */
/* supporto esterno  SD . Riceve i comandi dalla macchina             */
/* a stati principale , esegue e fornisce i risultati                 */
/*--------------------------------------------------------------------*/
// include comune librerie LANZI
#include "../Lanzi_library.h"

// variabili locali
static STORAGE_WORK FSM_storage;  // struttura lavoro gestione storage

static const char str_storage_init_start[] PROGMEM   = { "- Inizializzo FSM gestione storage ... -"};
static const char str_storage_init_end[]   PROGMEM   = { "- Fine inizializzazione Storage -"};

/*--------------------------------------------------*/
/* function storage_init()                          */
/*--------------------------------------------------*/
void storage_init(void)
  {
     debug_message_enable(TRUE,1);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_storage_init_start);
     
     FSM_storage.boot_flg = TRUE;
     
     debug_message_timestamp_PGM(str_storage_init_end);
  }
  
void storage_set_debug_opzion(BOOL ena,BOOL ident)
  {
     FSM_storage.debug_ena = ena;
     FSM_storage.debug_ident = ident;
  }

/*--------------------------------------------------*/
/* function storage_hand()                          */
/*--------------------------------------------------*/
void storage_hand(void)
  {
     uint8_t rcv_command;
     //BOOL debug_ena;
     
     // se siamo la prima volta dopo un reset
     // setta lo stato iniziale
     if (FSM_storage.boot_flg)
       {
          uint8_t temp_ident = FSM_storage.debug_ident;
          FSM_storage.debug_ident = 0;
          debug_print_FSM_storage(&FSM_storage,(char *)"avvio macchina a gestione storage ..");
          FSM_storage.debug_ident = temp_ident;
          FSM_storage.stat = ST_STORAGE_INIT;
          FSM_storage.boot_flg = FALSE;   
          return;     
       }       
       
     switch (FSM_storage.stat)
       {
          // condizione iniziale
          case ST_STORAGE_INIT:
            FSM_storage.stat = ST_STORAGE_WAIT_CMD;
            break;
            
          // attesa comandi
          case ST_STORAGE_WAIT_CMD:  
            rcv_command = FSM_storage.command; 
            if (rcv_command)
              {
                 // azzera comando ricevuto e risposta da dare
                 FSM_storage.command = 0;                
                 FSM_storage.answer = 0;
                 // forza flag prima scansione nello stato
                 FSM_storage.first = TRUE;
                 switch (rcv_command)
                    {
                       case CMD_STORAGE_CHK_APERTURA:
                         FSM_storage.stat = ST_STORAGE_CHK_APERTURA;
                         break;
                       case CMD_STORAGE_CHK_CHIUSURA:
                         FSM_storage.stat = ST_STORAGE_CHK_CHIUSURA;
                         break;
                       case CMD_STORAGE_CHK_RICARICA:
                         FSM_storage.stat = ST_STORAGE_CHK_RICARICA;
                         break;
                       case CMD_STORAGE_READ_RICARICA:
                         FSM_storage.stat = ST_STORAGE_READ_RICARICA;
                         break;
                       case CMD_STORAGE_FIND_BADGE:
                         FSM_storage.stat = ST_STORAGE_FIND_BADGE;
                         break;
                       case CMD_STORAGE_READ_ABIL:
                         FSM_storage.stat = ST_STORAGE_READ_ABIL;
                         break;
                       case CMD_STORAGE_SAVE_OFFLINE:
                         FSM_storage.stat = ST_STORAGE_WRITE_OFFLINE;
                         break;
                       case CMD_STORAGE_SAVE_TRANS:
                         FSM_storage.stat = ST_STORAGE_WRITE_MV;
                         break;
                    }
              } 
            break; 
                
          // verifica "apertura.dat"
          case ST_STORAGE_CHK_APERTURA:
            debug_print_FSM_storage(&FSM_storage,(char *)"ST_STORAGE_CHK_APERTURA");
            /*if (init_check_apertura() == FUNC_RET_DONE)
              {
                 debug_print_timestamp_ident(TRUE,4,(char *)"step10 ");
                 FSM_storage.answer = ANS_STORAGE_OK;
                 FSM_storage.stat = ST_STORAGE_WAIT_CMD;
              }*/
            break;
            
          // verifica "chiusura.dat"
          case ST_STORAGE_CHK_CHIUSURA: 
            debug_print_FSM_storage(&FSM_storage,(char *)"ST_STORAGE_CHK_CHIUSURA");
            /*if (init_check_chiusura() == FUNC_RET_DONE)
              {
                 FSM_storage.answer = ANS_STORAGE_OK;
                 FSM_storage.stat = ST_STORAGE_WAIT_CMD;
              }*/              
            break;
            
          // verifica "ricarica.dat"      
          case ST_STORAGE_CHK_RICARICA:
            debug_print_FSM_storage(&FSM_storage,(char *)"ST_STORAGE_CHK_RICARICA");
            /*if (init_check_ricarica() == FUNC_RET_DONE)
              {
                 FSM_storage.answer = ANS_STORAGE_OK;
                 FSM_storage.stat = ST_STORAGE_WAIT_CMD;
              }*/
            break;
            
          // leggi "ricarica.dat" 
          case ST_STORAGE_READ_RICARICA:
            debug_print_FSM_storage(&FSM_storage,(char *)"ST_STORAGE_READ_RICARICA");
            /*if (init_ricarica_read((char *)&FSM_storage.answer_data,STORAGE_ANS_SIZE) == FUNC_RET_DONE)
              {
                 FSM_storage.answer = ANS_STORAGE_OK;
                 FSM_storage.stat = ST_STORAGE_WAIT_CMD;
              }*/              
            break;
            
          // cerca il badge (in ANAG.DAT)
          case ST_STORAGE_FIND_BADGE:
            FUNC_RETVAL esito;

            esito = anagrafica_cerca_badge_offline((char *)FSM_storage.command_data,
                                                   FSM_storage.answer_data);
            // OK = lettura avvenuta (file esiste) 
            // in "FSM_storage.answer_data" c'e l'esito della ricerca 
            switch (esito)
              {
                 case FUNC_OK:
                   FSM_storage.answer = ANS_STORAGE_OK;
                   FSM_storage.stat = ST_STORAGE_WAIT_CMD;
                   break;
                 // tipicamente non trovato il file
                 case FUNC_ERROR:
                   FSM_storage.stat = ST_STORAGE_ERROR;
                   break;
                 case FUNC_FAIL:
                   FSM_storage.stat = ST_STORAGE_FAIL;
                   break;
                 default:
                   FSM_storage.stat = ST_STORAGE_SOFT_ERR;;
              }
            break; 
            
          // verifica le abilitazioni
          case ST_STORAGE_READ_ABIL:
            /*uint8_t settore;
            settore = erog_leggi_settore_corrente();
            if (verifica_abilitazioni((char *)FSM_storage.command_data,settore,FSM_storage.answer_data) == FUNC_RET_DONE)
              {
                 FSM_storage.answer = ANS_STORAGE_OK;
                 FSM_storage.stat = ST_STORAGE_WAIT_CMD;                
              }*/
            break;

          // Scrittura del file di log delle transazioni
          case ST_STORAGE_WRITE_MV:
            debug_print_FSM_storage(&FSM_storage,(char *)"ST_STORAGE_WRITE_MV");
            break;

          // Scrittura del file per la memorizzazione della transazione avvenuta offline
          case ST_STORAGE_WRITE_OFFLINE:
            debug_print_FSM_storage(&FSM_storage,(char *)"ST_STORAGE_WRITE_OFFLINE");
            //create_offline_file(FSM_storage.msg_data);
            break;

          case ST_STORAGE_SOFT_ERR:
            break;
          
          case ST_STORAGE_ERROR:
            FSM_storage.stat = ST_STORAGE_WAIT_CMD;
            break;
          
          case ST_STORAGE_FAIL:
            break;
       }
  }


/*---------------------------------------------------------*/
/* function storage_send_command()                         */
/* Invia un comando al gestore delle memorizzazioni su SD  */
/* Opzionalmente passa dei parametri in "data_addr" per    */
/* una dimensione "data_size" . Se non ci sono parametri   */
/* da passare "data_addr" vale NULLE e data_size = 0       */
/*---------------------------------------------------------*/
void storage_send_command(uint8_t command,uint8_t *data_addr,uint8_t data_size)
  {
     switch (command)
       {
          case  CMD_STORAGE_CHK_APERTURA:
          case  CMD_STORAGE_CHK_CHIUSURA:
          case  CMD_STORAGE_CHK_RICARICA:
          case  CMD_STORAGE_READ_RICARICA:
          case  CMD_STORAGE_FIND_BADGE:
          case  CMD_STORAGE_READ_ABIL:
          case  CMD_STORAGE_SAVE_TRANS:
            FSM_storage.command = command;
            // vedi se comando prevede parametri
            if (data_addr != NULL)
              {
                 // vedi se abbastanza spazio per copiare
                 if (data_size < sizeof(FSM_storage.command_data))
                   {
                      memcpy(FSM_storage.command_data,data_addr,data_size);
                   }  
              }
            break; 
          
          case  CMD_STORAGE_SAVE_OFFLINE:
            if (data_addr != NULL)
            {
              if (data_size < sizeof(FSM_storage.msg_data))
              {
                memcpy(FSM_storage.msg_data, data_addr, data_size);
              }
            }
            break;
          
          default:
            break;
       }
  }
  
   
/*---------------------------------------------------*/
/* function storage_get_answer()                     */
/* Fornisce la risposta al comando inviato           */
/*---------------------------------------------------*/
uint8_t storage_get_answer(void)
  {
     return FSM_storage.answer;
  } 
   
/*----------------------------------------*/
/* function storage_get_answer_OK()       */
/*----------------------------------------*/
/*BOOL storage_get_answer_OK(void)
  {
     if (FSM_storage.answer == ANS_STORAGE_OK)
       {
          return TRUE;
       }
     else
       {
          // verifica il timeout dell'operazione
          logica_macchina_hand_subtimeout(TRUE);
          return FALSE;
       }
  }*/
   
/*----------------------------------------*/
/* function storage_get_answer_data()     */
/*----------------------------------------*/
void storage_get_answer_data(uint8_t *resp_data,uint8_t resp_size)
  {
     memcpy(resp_data,FSM_storage.answer_data,resp_size);
  }
