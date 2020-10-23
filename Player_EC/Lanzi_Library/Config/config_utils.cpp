// include comune librerie LANZI
#include "../Lanzi_library.h"

/*#include "stdint.h"
#include "project_def.h"
#ifdef ENV_ARDUINO
#include "Arduino.h"
#endif
#include "My_Include/mydef.h"
#include "My_Include/global_define.h"
#include "My_Debug/Debug_Serial/debug_serial.h"
#include "My_Debug/Debug_Utility/debug_utility.h"
#include "My_Library/Utilities/CRC_func.h"
#include "My_Library/Utilities/AVR_API.h"
#include "config_def.h"
#include "config_utils.h"
#include "config_lib.h"
#include "config_API.h"
#include "config_decode.h"*/

// Prototipi funzione locali
static uint8_t config_get_numero_settori(void);
static uint8_t erog_get_dimensione_celle(uint8_t num_settore);
static void config_get_dati_settore(uint8_t settore, SEC_DATA* dati_settore);
static void config_visualizza_ID_macchina(void);
static void config_visualizza_dati_LAN(void);
static void visualizza_IP(MY_IP_ADDR ip, char* buff, uint8_t buf_size);
static void config_visualizza_dati_settore(void);
static void config_visualizza_impostazioni_macchina(void);
//static void config_decode_line(char* read_line, CONFIG_DAT* set_ptr);

// Stringhe per stampa di debug
const char str_separation_line[] PROGMEM = { "*=====================================*" };
const char str_config_print[] PROGMEM    = { "* visualizzazione SETUP macchina (RAM)*" };
const char str_machine_id[] PROGMEM      = { "-- idenficatori macchina --" };
const char str_lan_param[] PROGMEM       = { "-- parametri LAN --" };
const char str_sector_data[] PROGMEM     = { "-- visualizzazione dati settori da config --" };
const char str_machine_setup[] PROGMEM   = { "-- visualizzazione impostazioni macchina --" };



/*-----------------------------------------------------------*/
/* function config_decode_line_from_server()                 */
/* Decodifica una linea del file config come letta dalla SD  */
/*-----------------------------------------------------------*/
void config_decode_line_from_server(char* read_line, BOOL print_ena)
  {
     // vedi se visualizzazione per debug abilitata
     config_work.dbg_ena = print_ena;
     config_work.dbg_idn = 8;

     // decodifica effettivamente la linea
     config_decode_line(read_line, &setup_macchina);
  }


/*-----------------------------------------------------------*/
/* function config_decode_line_from_SD()                     */
/* Decodifica una linea del file config come letta dalla SD  */
/*-----------------------------------------------------------*/
void config_decode_line_from_SD(char *read_line,BOOL print_ena)
  {
     // vedi se visualizzazione per debug abilitata     
     config_work.dbg_ena = print_ena;
     config_work.dbg_idn = 6;
       
     // decodifica effettivamente la linea
     config_decode_line(read_line,&setup_macchina);
  }
//=========================================================================================//

void config_test(void)
  {
     // prima bisogna caricare il config dalla SD
     
     // verifica integrita strutturaa
     if (!config_verify())
       {
          // reinizializza ai valori di defalt
          config_init_data();

          // aggiorna il CRC
          config_update_CRC();

       }

#ifdef DEBUG_VERSION
     config_display_all();
#endif

 }

//===================================================================================//
void config_display_all(void)
{
    BOOL loc_ena = TRUE;
    uint8_t loc_idn = 2;

    debug_print_ena(loc_ena, (char*)"\n\r");
    debug_print_timestamp_ident(loc_ena, loc_idn, AVR_PGM_to_str(str_separation_line));
    debug_print_timestamp_ident(loc_ena, loc_idn, AVR_PGM_to_str(str_config_print));
    debug_print_timestamp_ident(loc_ena, loc_idn, AVR_PGM_to_str(str_separation_line));

    config_visualizza_ID_macchina();
    config_visualizza_dati_LAN();
    config_visualizza_dati_settore();
    config_visualizza_impostazioni_macchina();

    debug_print_timestamp_ident(loc_ena, loc_idn, AVR_PGM_to_str(str_separation_line));

    debug_print_ena(loc_ena, (char*)"\n\r");
}

/*---------------------------------------------*/
/* function config_visualizza_ID_macchina()    */
/*---------------------------------------------*/
static void config_visualizza_ID_macchina(void)
{
    char buff[80];
    BOOL loc_ena = TRUE;
    uint8_t loc_idn = 2;

    //debug_print_ena(loc_ena,(char *)"\n\r");
    debug_print_timestamp_ident(loc_ena, loc_idn, AVR_PGM_to_str(str_machine_id));

    // visualizza numero computer
    sprintf(buff, "numero_computer : %s", setup_macchina.computer_num);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    // visualizza nome computer
    sprintf(buff, "nome_computer   : %s", setup_macchina.computer_name);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);
}

/*-------------------------------------------*/
/* function config_visualizza_dati_LAN()     */
/*-------------------------------------------*/
static void config_visualizza_dati_LAN(void)
{
    char buff[80], ip_ASCII[20];
    BOOL loc_ena = TRUE;
    uint8_t loc_idn = 2;

    //debug_print_ena(loc_ena,(char *)"\n\r");
    debug_print_timestamp_ident(loc_ena, loc_idn, AVR_PGM_to_str(str_lan_param));

    visualizza_IP(setup_macchina.dati_LAN.IP_addr, ip_ASCII, sizeof(ip_ASCII));
    sprintf(buff, "IP_address     : %s", ip_ASCII);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    visualizza_IP(setup_macchina.dati_LAN.SM_addr, ip_ASCII, sizeof(ip_ASCII));
    sprintf(buff, "subnet_mask    : %s", ip_ASCII);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    visualizza_IP(setup_macchina.dati_LAN.GW_addr, ip_ASCII, sizeof(ip_ASCII));
    sprintf(buff, "gateway        : %s", ip_ASCII);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    visualizza_IP(setup_macchina.dati_LAN.DNS_addr, ip_ASCII, sizeof(ip_ASCII));
    sprintf(buff, "DNS_address    : %s", ip_ASCII);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    visualizza_IP(setup_macchina.dati_LAN.SERV_addr, ip_ASCII, sizeof(ip_ASCII));
    sprintf(buff, "server_address : %s", ip_ASCII);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    sprintf(buff, "MAC_address    : %02X-%02X-%02X-%02X-%02X-%02X",
        setup_macchina.dati_LAN.MAC[0], setup_macchina.dati_LAN.MAC[1],
        setup_macchina.dati_LAN.MAC[2], setup_macchina.dati_LAN.MAC[3],
        setup_macchina.dati_LAN.MAC[4], setup_macchina.dati_LAN.MAC[5]);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    sprintf(buff, "numero_porta   : %05u", setup_macchina.dati_LAN.portnum);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);
}

static void visualizza_IP(MY_IP_ADDR ip, char* buff, uint8_t buf_size)
{
    char temp[40];

    sprintf(temp, "%03d.%03d.%03d.%03d", ip.oct1, ip.oct2, ip.oct3, ip.oct4);
    strncpy(buff, temp, buf_size);
}

/*-------------------------------------------------*/
/* function config_visualizza_dati_settore()       */
/*-------------------------------------------------*/
static void config_visualizza_dati_settore(void)
{
    uint8_t sect;
    uint8_t num_sect;
    uint8_t dim_celle;
    SEC_DATA dati_settore;
    char buff[80];
    BOOL loc_ena = TRUE;
    uint8_t loc_idn = 2;

    //debug_print_ena(loc_ena,(char *)"\n\r");
    debug_print_timestamp_ident(loc_ena, loc_idn, AVR_PGM_to_str(str_sector_data));
    num_sect = config_get_numero_settori();
    for (sect = 1; sect <= num_sect; sect++)
    {
        config_get_dati_settore(sect, &dati_settore);
        dim_celle = erog_get_dimensione_celle(sect);
        sprintf(buff, "sett:%03d   piat:%03d start:%03d end :%03d ncel:%03d dcel:%03d",
            sect,
            dati_settore.piatto,
            dati_settore.pos_start,
            dati_settore.pos_end,
            dati_settore.n_celle,
            dim_celle);
        debug_print_timestamp_ident(loc_ena, loc_idn, buff);
    }
}

/*----------------------------------------------------*/
/* void config_visualizza_impostazioni_macchina()     */
/*----------------------------------------------------*/
static void config_visualizza_impostazioni_macchina(void)
{
    char buff[80];
    BOOL loc_ena = TRUE;
    uint8_t loc_idn = 2;

    //debug_print_ena(loc_ena,(char *)"\n\r");
    debug_print_timestamp_ident(loc_ena, loc_idn, AVR_PGM_to_str(str_machine_setup));

    sprintf(buff, "tempo lettura data_ora : %03u", setup_macchina.dati_macchina.clock_update_tim);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    sprintf(buff, "timeout selezione      : %03u", setup_macchina.dati_macchina.prod_select_tim);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    sprintf(buff, "badge_start            : %03u", setup_macchina.dati_macchina.badge_start);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    sprintf(buff, "badge_end              : %03u", setup_macchina.dati_macchina.badge_end);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    sprintf(buff, "tempo_standby          : %03u", setup_macchina.dati_macchina.LED_standby);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

    sprintf(buff, "limite corrente        : %03u", setup_macchina.dati_macchina.taglio_motore);
    debug_print_timestamp_ident(loc_ena, loc_idn, buff);

}

/*----------------------------------------------------*/
/* function config_verify()                           */
/* Verifica l'integrita del config .                  */
/* Restituisce TRUE se il config e' corretto  e sono  */
/* presenti gli ID di inizio e fine                   */
/*----------------------------------------------------*/
BOOL config_verify(void)
{
    uint16_t calc_CRC;
    BOOL retval;
    uint8_t id_ini, id_end;

    // verifica marca di inizio 
    if (setup_macchina.marca_ini == 0x55AA)
        id_ini = 1;
    else
        id_ini = 0;

    // verifica marca di fine  
    if (setup_macchina.marca_end == 0xC034)
        id_end = 1;
    else
        id_end = 0;

    if ((id_ini == 1) && (id_end == 1))
    {
        // calcola il CRC della struttura
        calc_CRC = calcola_CRC_MDB((void*)&setup_macchina.computer_num[0], sizeof(CONFIG_DAT) - 4);
        // vedi se CRC valido
        if (calc_CRC == setup_macchina.CRC)
            retval = TRUE;
        else
            retval = FALSE;
    }
    else//
        retval = FALSE;

    char buff[80];
    sprintf(buff, "ID:%d%d calc_CRC:%04x  config_CRC:%04x", id_ini, id_end, calc_CRC, setup_macchina.CRC);
    debug_print_timestamp_ident(TRUE, 2, buff);

    return retval;
}

/*---------------------------------------------------*/
/* function config_update_CRC()                      */
/* Aggiorna il CRC del config                        */
/*---------------------------------------------------*/
void config_update_CRC(void)
  {
     uint16_t calc_CRC;

     // inizializza i marcatori di inizio e fine
     setup_macchina.marca_ini = 0x55AA;
     setup_macchina.marca_end = 0xC034;

     //setup_macchina.boot_mode = boot_mode;

     // calcola il CRC
     calc_CRC = calcola_CRC_MDB((void*)&setup_macchina.computer_num[0], sizeof(CONFIG_DAT) - 4);

     // e memorizzalo
     setup_macchina.CRC = calc_CRC;
  }


/*---------------------------------------------------*/
/* function config_init_data()                       */
/* Inizializza i valori default del config in caso   */
/* di prima volta o dati alterati                    */
/*---------------------------------------------------*/
void config_init_data(void)
  {
     memset(&setup_macchina, 0, sizeof(CONFIG_DAT));

     // da eliminare
     strcpy(setup_macchina.computer_num, "10100");

     setup_macchina.marca_ini = 0x55AA;
     setup_macchina.marca_end = 0xC034;
  }

/*-----------------------------------------------------------------*/
/* function config_get_numero_settori()                            */
/* Legge dal config il numero di settori di questa configurazione  */
/*-----------------------------------------------------------------*/
static uint8_t config_get_numero_settori(void)
  {
     return setup_macchina.num_settori;
  }

/*---------------------------------------------------------------------------*/
/* function config_get_numero_celle()                                        */
/* Legge dal config i dati (piatto,inizio,fine,ncelle) del settore indicato  */
/*---------------------------------------------------------------------------*/
static void config_get_dati_settore(uint8_t settore, SEC_DATA* dati_settore)
  {
     *dati_settore = setup_macchina.dati_settori[settore - 1];
  }

/*----------------------------------------------------------------*/
/* functione erogazione_get_dimensione_celle()                    */
/* Legge la dimensione delle celle (espresso in numero di denti)  */
/* del settore corrente                                           */
/*----------------------------------------------------------------*/
static uint8_t erog_get_dimensione_celle(uint8_t num_settore)
  {
     return 0;//erog_data.dimensione_celle[num_settore - 1];
  }

//===========================================================================//


/*---------------------------------------------------------*/
/* Utility libreria "config" per visualizzare una stringa  */
/* preceduta ad una tag identificativa                     */
/* nb: la tag identificativa e' sempre in program memory   */
/*---------------------------------------------------------*/
void config_debug_print(char *message)
  {
     debug_message_tag_ident(str_log_config_tag,message);
  }  

/*---------------------------------------------------------*/
/* Utility libreria "config" per visualizzare una stringa  */
/* (in program memory) preceduta ad una tag identificativa */
/* nb: la tag identificativa e' sempre in program memory   */
/*---------------------------------------------------------*/
void config_debug_print_PGM(const char *message)
  {
     debug_message_tag_ident_PGM(str_log_config_tag,message);
  }  


//===========================================================================//

