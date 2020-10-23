// include comune librerie LANZI
#include "../Lanzi_library.h"

// prototipi funzione locali
static void config_estrai_stringa(char* line_input, char* stringa, uint8_t size_stringa);
static void config_estrai_numero(char* line_input, uint16_t* numero);
static void config_estrai_IP(char* line_input, MY_IP_ADDR* ip_addr);
static void config_estrai_MAC(char* line_input, uint8_t* MAC_addr);
static void config_estrai_dati_settore(char* line_input, SEC_DATA* dato_settore, uint16_t* currset);
static BOOL estrai_valore(char* line_input, char* valore, uint8_t val_size);

// Stringhe per salvataggio dati config
const char cfg_str_start[] PROGMEM     = { "#*" };
const char cfg_str_PAY[] PROGMEM       = { "#PAY" };
const char cfg_str_CFG[] PROGMEM       = { "#CFG" };
const char cfg_str_NUM_COMP[] PROGMEM  = { "NUM_COMP" };
const char cfg_str_NOME_COMP[] PROGMEM = { "NOME_COM" };
const char cfg_str_IP_MACC[] PROGMEM   = { "IP_MACC" };
const char cfg_str_SUBNET[] PROGMEM    = { "SUBNET" };
const char cfg_str_GATEWAY[] PROGMEM   = { "GATEWAY" };
const char cfg_str_DNS[] PROGMEM       = { "DNS" };
const char cfg_str_MAC[] PROGMEM       = { "MAC" };
const char cfg_str_IPSERVER[] PROGMEM  = { "IPSERVER" };
const char cfg_str_PORT[] PROGMEM      = { "PORT" };
const char cfg_str_NUM_SETT[] PROGMEM  = { "NUM_SETT" };
const char cfg_str_SEC[] PROGMEM       = { "SEC" };
const char cfg_str_UPD_CLK[] PROGMEM   = { "UPD_CLK" };
const char cfg_str_TIM_BADG[] PROGMEM  = { "TIM_BADG" };
const char cfg_str_UNUSED1[] PROGMEM   = { "UNUSED1" };
const char cfg_str_STR_BADG[] PROGMEM  = { "STR_BADG" };
const char cfg_str_END_BADG[] PROGMEM  = { "END_BADG" };
const char cfg_str_AUTOJUMP[] PROGMEM  = { "AUTOJUMP" };
const char cfg_str_STAND_BY[] PROGMEM  = { "STAND_BY" };
const char cfg_str_IMOT_P0[] PROGMEM   = { "IMOT_P0"  };
const char cfg_str_IMOT_MOV[] PROGMEM  = { "IMOT_MOV" };
const char cfg_str_RIT_SENS[] PROGMEM  = { "RIT_SENS" };
const char cfg_str_MOT_CUT[] PROGMEM   = { "MOT_CUT" };
const char cfg_str_MOT_DEL[] PROGMEM   = { "MOT_DEL" };
const char cfg_str_RIT_EM[] PROGMEM    = { "RIT_EM" };
const char cfg_str_UNUSED2[] PROGMEM   = { "UNUSED2" };

/*----------------------------------------------------*/
/* function config_decode_line()                      */
/* Legge,analizza, e memorizza una riga del config    */
/*----------------------------------------------------*/
void config_decode_line(char* read_line, CONFIG_DAT* set_ptr)
  {
     static uint16_t num_sect;
     static uint16_t curr_sect;
     SEC_DATA tmp_sector;
     char buff[120];
     uint8_t i;

     // se e' un commento esci 
     if ((strncmp(read_line, AVR_PGM_to_str(cfg_str_start), 2) == 0) ||
         (strncmp(read_line, AVR_PGM_to_str(cfg_str_PAY), 4) == 0) ||
         (strncmp(read_line, AVR_PGM_to_str(cfg_str_PAY), 4) == 0))
       {
          //debug_print_ena(decode_print_ena,"\n\r");
          #ifdef DEBUG_VERSION
          debug_print_timestamp_ident(config_work.dbg_ena, config_work.dbg_idn, read_line);
          #endif

          return;
       }

    // visualizza stringa ricevuta 
    memset(buff, 0, sizeof(buff));
    strncpy(buff, read_line, sizeof(buff) - 2);
    for (i = strlen(buff); i < 26; i++)
        buff[i] = ' ';
    strncat(buff, " -> ", sizeof(buff) - 2);

#ifdef DEBUG_VERSION
    debug_print_timestamp_ident(config_work.dbg_ena, config_work.dbg_idn, buff);
#endif

    //---------------------------------------------------------//
    //   estrai le informazioni del gruppo "MACHINE_ID"        //
    //---------------------------------------------------------//

    // numero computer
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_NUM_COMP), 7) == 0)
    {
        config_estrai_stringa(read_line, set_ptr->computer_num, sizeof(set_ptr->computer_num));
        return;
    }

    // nome computer
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_NOME_COMP), 7) == 0)
    {
        config_estrai_stringa(read_line, set_ptr->computer_name, sizeof(set_ptr->computer_name));
        return;
    }

    //---------------------------------------------------------//
    //   estrai le informazioni del gruppo "LAN_CONFIG"        //
    //---------------------------------------------------------//
    // ip macchina
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_IP_MACC), 7) == 0)
    {
        config_estrai_IP(read_line, &set_ptr->dati_LAN.IP_addr);
        return;
    }

    // subnet  
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_SUBNET), 6) == 0)
    {
        config_estrai_IP(read_line, &set_ptr->dati_LAN.SM_addr);
        return;
    }

    // gateway
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_GATEWAY), 7) == 0)
    {
        config_estrai_IP(read_line, &set_ptr->dati_LAN.GW_addr);
        return;
    }

    // dns (non utilizzato)
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_DNS), 3) == 0)
    {
        config_estrai_IP(read_line, &set_ptr->dati_LAN.DNS_addr);
        return;
    }

    // MAC address
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_MAC), 3) == 0)
    {
        config_estrai_MAC(read_line, &set_ptr->dati_LAN.MAC[0]);
        return;
    }

    // ip server
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_IPSERVER), 8) == 0)
    {
        config_estrai_IP(read_line, &set_ptr->dati_LAN.SERV_addr);
        return;
    }

    // numero porta
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_PORT), 4) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_LAN.portnum);
        return;
    }

    //---------------------------------------------------------//
    //   estrai le informazioni del gruppo "SECTOR_DATA"       //
    //---------------------------------------------------------//    
    // numero settori usati  
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_NUM_SETT), 8) == 0)
    {
        config_estrai_numero(read_line, &num_sect);
        if (num_sect <= MAX_SETTORI)
            set_ptr->num_settori = num_sect;
        return;
    }

    // dati singolo settore (ripetuti per "num_sect")   
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_SEC), 3) == 0)
    {
        config_estrai_dati_settore(read_line, &tmp_sector, &curr_sect);
        if (curr_sect <= num_sect)
        {
            set_ptr->dati_settori[curr_sect - 1] = tmp_sector;
        }
        return;
    }

    //---------------------------------------------------------//
    //   estrai le informazioni del gruppo "SETUP"             //
    //---------------------------------------------------------//            
    // tempo aggiornamento orologio          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_UPD_CLK), 7) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.clock_update_tim);
        return;
    }

    // timeout dopo passaggio badge          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_TIM_BADG), 8) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.prod_select_tim);
        return;
    }

    // non usato          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_UNUSED1), 7) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.offline_timeout);
        return;
    }

    // primo carattere badge da considerare          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_STR_BADG), 8) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.badge_start);
        return;
    }

    // ultimo carattere badge da considerare            
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_END_BADG), 8) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.badge_end);
        return;
    }

    // abilita autojump          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_AUTOJUMP), 8) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.auto_jump);
        return;
    }

    // minuti per stand-by          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_STAND_BY), 8) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.LED_standby);
        return;
    }

    // corrente motore in posizionamento a zero          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_IMOT_P0), 7) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.livello_1);
        return;
    }

    // corrente motore in movimenti normali          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_IMOT_MOV), 8) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.livello_2);
        return;
    }

    // ms ritardo attivazione sensore corrente          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_RIT_SENS), 8) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.ritardo_taglio_motore);
        return;
    }

    // taglio motore          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_MOT_CUT), 7) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.taglio_motore);
        return;
    }

    // ritardo motore          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_MOT_DEL), 7) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.ritardo_stop_motore);
        return;
    }

    // tempo attivazione elettroserrature          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_RIT_EM), 6) == 0)
    {
        config_estrai_numero(read_line, &set_ptr->dati_macchina.look_door_tim);
        return;
    }

    // non usato          
    if (strncmp(read_line, AVR_PGM_to_str(cfg_str_UNUSED2), 7) == 0)
    {
        return;
    }

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// FUNZIONI DI APPOGGIO DELLA config_decode_line() ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------*/
/* function config_estrai_stringa()                */
/* Legge una riga dal config e estrae una stringa  */
/*-------------------------------------------------*/
static void config_estrai_stringa(char* line_input, char* stringa, uint8_t size_stringa)
{
    char valore[30];
    char buff[80];

    // separa il valore dalla stringa di ingresso
    if (estrai_valore(line_input, valore, sizeof(valore)))
    {
        // estrai il contenuto della stringa
        strncpy(stringa, valore, size_stringa);
        // vedi se visualizzazione abilitata
#ifdef DEBUG_VERSION
        if (config_work.dbg_ena)
        {
            sprintf(buff, "( %s )", valore);
            debug_print_ena(TRUE, buff);
        }
#endif
    }
}

/*-------------------------------------------------*/
/* function config_estrai_numero()                 */
/* Legge una riga dal config e estrae un numero    */
/*-------------------------------------------------*/
static void config_estrai_numero(char* line_input, uint16_t* numero)
{
    char valore[30];
    char buff[80];

    // separa il valore dalla stringa di ingresso
    if (estrai_valore(line_input, valore, sizeof(valore)))
    {
        // leggi il numero
        *numero = atoi(valore);
        // vedi se visualizzazione abilitata
#ifdef DEBUG_VERSION
        if (config_work.dbg_ena)
        {
            sprintf(buff, "(%05u)", *numero);
            debug_print_ena(TRUE, buff);
        }
#endif
    }
}

/*-------------------------------------------------*/
/* function config_estrai_IP()                     */
/* Legge una riga dal config e estrae una stringa  */
/*-------------------------------------------------*/
static void config_estrai_IP(char* line_input, MY_IP_ADDR* ip_addr)
{
    char valore[30], temp[6];
    char buff[60];

    // separa il valore dalla stringa di ingresso
    if (estrai_valore(line_input, valore, sizeof(valore)))
    {

        if ((strlen(valore) == 15) && (valore[3] == '.') && (valore[7] == '.') && (valore[11] == '.'))
        {
            memset(temp, 0, sizeof(temp));
            // estrai il valore del primo ottetto
            strncpy(temp, &valore[0], 3);
            ip_addr->oct1 = atoi(temp);

            // estrai il valore del secondo ottetto
            strncpy(temp, &valore[4], 3);
            ip_addr->oct2 = atoi(temp);

            // estrai il valore del terzo ottetto
            strncpy(temp, &valore[8], 3);
            ip_addr->oct3 = atoi(temp);

            // estrai il valore del quarto ottetto
            strncpy(temp, &valore[12], 3);
            ip_addr->oct4 = atoi(temp);

            // vedi se visualizzazione abilitata
#ifdef DEBUG_VERSION
            if (config_work.dbg_ena)
            {
                sprintf(buff, "(%03d:%03d:%03d:%03d)",
                    ip_addr->oct1, ip_addr->oct2, ip_addr->oct3, ip_addr->oct4);
                debug_print_ena(TRUE, buff);
            }
#endif
        }
    }
}

static void config_estrai_MAC(char* line_input, uint8_t* MAC_addr)
{
    char valore[30];
    char buff[60];

    // separa il valore dalla stringa di ingresso
    if (estrai_valore(line_input, valore, sizeof(valore)))
    {
        if (strlen(valore) == 6)
        {
            // salva il mac address
            MAC_addr[0] = valore[0];
            MAC_addr[1] = valore[1];
            MAC_addr[2] = valore[2];
            MAC_addr[3] = valore[2];
            MAC_addr[4] = valore[3];
            MAC_addr[5] = valore[4];
            // vedi se visualizzazione abilitata
#ifdef DEBUG_VERSION
            if (config_work.dbg_ena)
            {
                sprintf(buff, "(%02x-%02x-%02x-%02x-%02x-%02x)",
                    MAC_addr[0], MAC_addr[1], MAC_addr[2],
                    MAC_addr[3], MAC_addr[4], MAC_addr[5]);
                debug_print_ena(TRUE, buff);
            }
#endif
        }
    }
}

/*-------------------------------------------------------------------*/
/* function config_estrai_dato_settore()                             */
/* Legge una riga dal config e estrae le informazioni di un settore  */
/*-------------------------------------------------------------------*/
static void config_estrai_dati_settore(char* line_input, SEC_DATA* dato_settore, uint16_t* currset)
{
    char valore[30];
    char num_sett_str[4];
    char piatto_str[4];    // piatto a cui appartiene il settore
    char pos_start_str[4]; // posizione inizio (numero dente)
    char pos_end_str[4];   // posizione fine   (numero dente)
    char n_celle_str[4];   // numero di celle nel settore
    char buff[100];

    memset(num_sett_str, 0, sizeof(num_sett_str));
    memset(piatto_str, 0, sizeof(piatto_str));
    memset(pos_start_str, 0, sizeof(pos_start_str));
    memset(pos_end_str, 0, sizeof(pos_end_str));

    // separa il numero del settore
    strncpy(num_sett_str, &line_input[3], 3);
    *currset = atoi(num_sett_str);

    // separa il valore dalla stringa di ingresso
    if (estrai_valore(line_input, valore, sizeof(valore)))
    {
        //001,000,035,006
        // esegui controlli preliminari string
        if ((strlen(valore) == 15) && (valore[3] == ',') && (valore[7] == ',') && (valore[11] == ','))
        {
            // estrai il valore del piatto
            strncpy(piatto_str, &valore[0], 3);
            dato_settore->piatto = atoi(piatto_str);

            // estrai il valore dell'inizio
            strncpy(pos_start_str, &valore[4], 3);
            dato_settore->pos_start = atoi(pos_start_str);

            // estrai il valore della fine
            strncpy(pos_end_str, &valore[8], 3);
            dato_settore->pos_end = atoi(pos_end_str);

            // estrai numero celle
            strncpy(n_celle_str, &valore[12], 3);
            dato_settore->n_celle = atoi(n_celle_str);

            // vedi se visualizzazione abilitata
#ifdef DEBUG_VERSION
            if (config_work.dbg_ena)
            {
                sprintf(buff, "(s%02d = pi%03d-in%03d-en%03d-ce%03d)",
                    *currset, dato_settore->piatto, dato_settore->pos_start,
                    dato_settore->pos_end, dato_settore->n_celle);
                debug_print_ena(TRUE, buff);
            }
#endif
        }
    }
}

/*------------------------------------------------*/
/* function estrai_valore()                       */
/* Separa il "valore" dalla stringa di ingresso   */
/*------------------------------------------------*/
static BOOL estrai_valore(char* line_input, char* valore, uint8_t val_size)
{
    char* pos_valore;

    // cerca il separatore tra chiave e valore
    pos_valore = strstr(line_input, " = ");
    // vedi se trovato
    if (pos_valore != NULL)
    {
        // estrai il valore
        strncpy(valore, pos_valore + 3, val_size);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



