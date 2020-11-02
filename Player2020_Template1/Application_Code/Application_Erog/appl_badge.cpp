/****************************************************************************/
/* File:           appl_badge.cpp                                           */
/* Data creazione: 08/05/2020  												*/
/* Creato da:      Ivan De Stefani											*/
/* Funzione:       Modulo di gestione della lettura dei badge      			*/
/*																			*/
/* Questo modulo contiene la macchina a stati di gestione della lettura 	*/
/* dei badge, del loro riconoscimento tramite invio di richiesta al gestore */
/* del server o lettura del file su SD assegnato ad essi, e trasposizione   */
/* delle informazioni all'interno di un vettore di booleani.                */
/*																			*/
/****************************************************************************/

#include "player_header.h"

FSM_WORK FSM_badge;		      // Macchina a stati per la gestione della logica di funzionamento dell'identificazione tramite badge

BOOL auth_vec[MAX_SEC_NUM + 3];   // Vettore di booleani per salvataggio informazioni di abilitazione prelievo da singolo settore
// minima dimensione per validita del badge
#define BADGE_MIN_SIZE 3

// Variabili locali per la memorizzazione dei dati del badge
static char badge[MAX_BADGE_SIZE];			// Scansione badge lettura
static char badge_read_all[MAX_BADGE_SIZE]; // Scansione badge senza filtro caratteri di controllo
BOOL auth_flag = FALSE;					    // Flag per determinare la differenza fra una scansione "normale" ed una per autorizzazione
BOOL badge_auth_ena = FALSE;				// Abilitazione del badge all'autorizzazione degli altri badge al prelievo di prodotti non abilitati

static BOOL badge_read;  // badge letto e preverifcato 
static BOOL badge_ena;   // abilitazione alla lettura

// Vettore per il nome degli stati di gestione del badge
char* badge_state_name[] = {
	"ST_BADGE_INIT",
	"ST_BADGE_READY",
	"ST_BADGE_READING",
	"ST_BADGE_DONE",
	"ST_BADGE_ERROR",
	"ST_BADGE_FAIL"
};

// Stringhe su program memory per messaggi di debug

const char str_bdg_st_init[] PROGMEM         = { "ST_BADGE_INIT" };
const char str_bdg_st_ready[] PROGMEM        = { "ST_BADGE_READY" };
const char str_bdg_st_reading[] PROGMEM      = { "ST_BADGE_READING" };
const char str_bdg_st_get_info[] PROGMEM     = { "ST_BADGE_DONE" };
const char str_bdg_st_error[] PROGMEM        = { "ST_BADGE_ERROR" };
const char str_bdg_st_fail[] PROGMEM         = { "ST_BADGE_FAIL" };
const char str_bdg_init_start[] PROGMEM      = { "Inizio badge_init..." };
const char str_bdg_init_end[] PROGMEM        = { "Fine badge_init!" };
const char str_bdg_scan[] PROGMEM            = { "   LETTURA  BADGE   " };
const char str_bdg_verifying[] PROGMEM       = { "Verifica in corso..." };
const char str_bdg_badge[] PROGMEM           = { "Badge: " };
const char str_bdg_debug_scan[] PROGMEM      = { "Scansionato badge " };

// prototipi funzione locali
static uint16_t badge_read_char(void);
static void badge_concat(char* vect, uint8_t size, char rx_chr);
static void badge_concat_all(char* vect, uint8_t size, char rx_chr);
static void badge_clear(char* vect, uint8_t size);
static void badge_print(void);
static BOOL badge_verify(char* badge);



/* restituisce TRUE se vi e' un badge letto e ha passato le */
/* verifiche preliminari di congruita                       */
BOOL badge_present(void)
  {
	 return badge_read;
  }

/* legge il contenuto dell'ultimo badge passato  */
void badge_get_last_reading(char* read_badge, uint8_t size)
  {
	 strncpy(read_badge, badge, size - 1);
  }

/****************************************************/
/* badge_init()                                     */
/*__________________________________________________*/
/* Funzione che inizializza la macchina a stati per */
/* la gestione dell'identificazione tramite badge   */
/* Parametri: nessuno                               */
/****************************************************/
void badge_init(void)
{
	debug_print_timestamp(bdg_dbg, AVR_PGM_to_str(str_bdg_init_start));

	badge_init_LL(9600L);

	
	fsm_init(&FSM_badge,
		     ST_BADGE_INIT,
		     badge_state_name,
			 MAX_BADGE_STATE,
			 (char *)"BDG");

	debug_print_timestamp(bdg_dbg, AVR_PGM_to_str(str_bdg_init_end));
}


/*********************************************************/
/* badge_check_server()                                  */
/*_______________________________________________________*/
/* Funzione che invia al gestore del server la richiesta */
/* per ottenere le informazione relative al badge        */
/* Parametri: badge da analizzare                        */
/*********************************************************/
/*void badge_check_server(char* badge)
{
	if (auth_flag)
	{
		if (Server_request_send(SRV_EV_SEND_AND_WAIT, SRV_MSGID_BADGE, badge_read_all))
		{
			if (get_badge_info(badge_read_all, auth_vec))
			{
				badge_auth_ena = TRUE;
			}
		}
	}
	else
	{
		Server_request_send(SRV_EV_SEND_AND_WAIT, SRV_MSGID_BADGE, badge);
		{
			get_badge_info(badge, auth_vec);
		}
	}

}*/


void badge_ena_read(void)
  {
	 debug_print_timestamp(DBG_ALWAYS_ON, (char*)"* lettura badge abilitata *");
	 while (badge_read_char() != BADGE_INVALID_CHR);
	 badge_ena = TRUE;
  }

void badge_dis_read(void)
  {
   	 debug_print_timestamp(DBG_ALWAYS_ON, (char*)"* lettura badge disabilitata *");
	 badge_ena = FALSE;
  }

/**************************************************/
/* badge_hand()                                   */
/*________________________________________________*/
/* Funzione che gestisce la lettura dei badge, la */
/* loro decodifica, e passa all' "appl_erog" le   */
/* informazioni necessarie.                       */
/* Parametri: nessuno                             */
/**************************************************/
void badge_hand(void)
{
	static uint32_t start_read;
	switch (FSM_badge.state)
	{
		case ST_BADGE_INIT:
			if (fsm_first_scan(&FSM_badge, AVR_PGM_to_str(str_bdg_st_init)))
			{
				fsm_set_state(&FSM_badge, ST_BADGE_READY);
			}
		break;

		case ST_BADGE_READY:
		  // attendi consenso alla lettura
		  if (badge_ena && badge_char_available())
			{
			   fsm_set_state(&FSM_badge, ST_BADGE_READING);			  
		    }
		  else
		    {
			   // lettura di eventuali caratteri presenti nel buffer
			   // della seriale dovuta a passaggi di badge in
			   // momenti non previsti
			   (void)badge_read_char();
		    }
		  break;

		case ST_BADGE_READING:
		  if (fsm_first_scan(&FSM_badge, AVR_PGM_to_str(str_bdg_st_reading)))
			{
				debug_print_timestamp(TRUE, "inizio lettura badge");
				badge_read = FALSE;
				fsm_set_timer(&FSM_badge, FSM_TIMER1);
				// azzera buffer locali
				badge_clear(badge_read_all, sizeof(badge_read_all));
				badge_clear(badge, sizeof(badge));
			}
		  // attesa finestra lettura
		  if (!fsm_timer_hand(&FSM_badge, FSM_TIMER1, 1000L))
		    {
			   uint16_t temp_chr;
			   temp_chr = badge_read_char();
			   // vedi se arrivato un carattere
			   if (temp_chr != 0xFFFF)
			   {
				  // inseriscilo nel buffer
				  badge_concat(badge, sizeof(badge), (char)temp_chr);
				  badge_concat_all(badge_read_all, sizeof(badge_read_all), (char)temp_chr);
			   }

		    }
		  else
			{
			   debug_print_timestamp(TRUE, "fine lettura badge");
			   badge_print();
			   if (badge_verify(badge))
			     {
				    badge_read = TRUE;
				    fsm_set_state(&FSM_badge, ST_BADGE_DONE);
			     }
			   else
			     {
				    fsm_set_state(&FSM_badge, ST_BADGE_ERROR);
			     }
			}
		  break;

		case ST_BADGE_DONE:
		  fsm_set_state(&FSM_badge, ST_BADGE_READY);
		  break;

		case ST_BADGE_ERROR:
			fsm_set_state(&FSM_badge, ST_BADGE_READY);
			break;

		case ST_BADGE_FAIL:
			break;

		default:
			;
	}
}


static void badge_print(void)
  {
	char buff[90];

	sprintf(buff, "badge         [%s]", badge);
	debug_print_timestamp(DBG_ALWAYS_ON, buff);

	strncpy(buff, "badge all_chr ", sizeof(buff) - 1);
	debug_make_buff_hex(&buff[strlen(buff)],sizeof(buff),(uint8_t *)badge_read_all, sizeof(badge_read_all));
	debug_print_timestamp(DBG_ALWAYS_ON, buff);

  }


static BOOL badge_verify(char* badge)
  {
	 if (strlen(badge) < BADGE_MIN_SIZE)
	 	return FALSE;
	 // if bla bla bla
	 return TRUE;
  }

/*********************************************************/
/* request_auth_check()                                  */
/*_______________________________________________________*/
/* Funzione che informa il gestore del badge che chi la  */
/* ha chiamata sta verificando se il badge passato       */
/* appartiene ad un autorizzatore                        */
/* Parametri: nessuno                                    */
/*********************************************************/

void request_auth_check(void)
{
	auth_flag = TRUE;

}


/*******************************************************************/
/* void badge_concat(char * vect)                                  */
/*_________________________________________________________________*/
/* Funzione che concatena nel vettore vect il badge letto dalla    */
/* seriale hardware Serial2.                                       */
/* Parametri: vettore in cui salvare il badge letto.               */
/*******************************************************************/
static void badge_concat(char *vect,uint8_t size,char rx_chr)
  {
	 uint8_t pos;
	 pos = strlen(vect);
	 
	 if (pos < (size - 1))
	   {
		 switch (rx_chr)
		   {
		      case 0x0A: // LF
			  case 0x0D: // CR
				break;
			  default:
				// elimina eventuali caratteri di controllo
				if (!iscntrl(rx_chr))
				  {
					 // qui il carattere e' ripulito
					 vect[pos] = rx_chr;
				  }
		   }

	   }
  }

/* versione di "badge_concat" senza filtro caratteri spuri */
/* cattura fisicamente ogni carattere ricevuto (x test)    */
static void badge_concat_all(char* vect, uint8_t size, char rx_chr)
{
	uint8_t pos;
	pos = strlen(vect);

	if (pos < (size - 1))
	  {
	    vect[pos] = rx_chr;
	  }
}

void badge_clear(char* vect, uint8_t size)
  {
	 memset(vect, 0, size);
  }
//====================================================================================================//

