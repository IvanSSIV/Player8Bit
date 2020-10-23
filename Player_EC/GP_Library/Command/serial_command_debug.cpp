#define MODULE_SERIAL_COMMAND_DEBUG

// include comuni in libreria "GP_library"
#include "..\GP_library.h"


// Definizioni mnemoniche comandi da seriale
#define CONFIG_PRINT   'C'        // C
#define TEST_PRINT     'T'        // T
#define DEBUG_ENABLE   'K'        // K
#define DEBUG_DISABLE  'D'        // D
#define RTC_DBG        'R'        // R
#define SD_DBG         'S'        // S
#define LCD_DBG        'L'        // L
#define ETH_DBG        'E'        // E
#define SRV_DBG        'V'        // V
#define MOT_DBG        'M'        // M
#define LGC_DBG        'G'        // G
#define PLC_DBG        'P'        // P
#define EEP_DBG        'X'        // X
#define BDG_DBG        'B'        // B
#define SER_DBG        'A'        // A
#define IO_DBG         'Z'        // Z
#define FSM_DBG        'F'             // F
#define INFO_DBG       'W'        // W
#define RTC_DISAL      'Y'        // Y
#define EROG_DBG       'H'       // H

static const char str_dbg_ena_all[] PROGMEM     = { "Accendo tutto il debug." };
static const char str_dbg_disena_all[] PROGMEM  = { "Spengo tutto il debug." };
static const char str_dbg_ena_RTC[] PROGMEM     = { "Spengo debug RTC." };
static const char str_dbg_disena_RTC[] PROGMEM  = { "Accendo debug RTC." };
static const char str_dbg_ena_SD[] PROGMEM      = { "Accendo debug SD." };
static const char str_dbg_disena_SD[] PROGMEM   = { "Spengo debug SD." };
static const char str_dbg_ena_LCD[] PROGMEM     = { "Accendo debug LCD." };
static const char str_dbg_disena_LCD[] PROGMEM  = { "Spengo debug LCD." };
static const char str_dbg_ena_ETH[] PROGMEM     = { "Accendo debug Ethernet." };
static const char str_dbg_disena_ETH[] PROGMEM  = { "Spengo debug Ethernet." };
static const char str_dbg_ena_SRV[] PROGMEM     = { "Accendo debug server." };
static const char str_dbg_disena_SRV[] PROGMEM  = { "Spengo debug server." };
static const char str_dbg_ena_MOT[] PROGMEM     = { "Accendo debug motore." };
static const char str_dbg_disena_MOT[] PROGMEM  = { "Spengo debug motore." };
static const char str_dbg_ena_LOG[] PROGMEM     = { "Accendo debug logica." };
static const char str_dbg_disena_LOG[] PROGMEM  = { "Spengo debug logica." };
static const char str_dbg_ena_PLC[] PROGMEM     = { "Accendo debug PLC." };
static const char str_dbg_disena_PLC[] PROGMEM  = { "Spengo debug PLC." };
static const char str_dbg_ena_EEP[] PROGMEM     = { "Accendo debug EEPROM." };
static const char str_dbg_disena_EEP[] PROGMEM  = { "Spengo debug EEPROM." };
static const char str_dbg_ena_BDG[] PROGMEM     = { "Accendo debug badge." };
static const char str_dbg_disena_BDG[] PROGMEM  = { "Spengo debug badge." };
static const char str_dbg_ena_SER[] PROGMEM     = { "Accendo debug comandi seriali." };
static const char str_dbg_disena_SER[] PROGMEM  = { "Spengo debug comandi seriali." };
static const char str_dbg_ena_GPIO[] PROGMEM    = { "Accendo debug GPIO." };
static const char str_dbg_disena_GPIO[] PROGMEM = { "Spengo debug GPIO." };
static const char str_dbg_ena_FSM[] PROGMEM     = { "Accendo debug FSM." };
static const char str_dbg_disena_FSM[] PROGMEM  = { "Spengo debug FSM." };
static const char str_all_dbg_ON[] PROGMEM      = { "Accendo tutto il debug." };
static const char str_all_dbg_OFF[] PROGMEM     = { "Spengo tutto il debug." };
const char str_dbg_ena_EROG[] PROGMEM    = { "Accendo debug erogazione." };
const char str_dbg_disena_EROG[] PROGMEM = { "Spengo debug erogazione." };

// Funzione che disallinea l'orologio per test
extern void RTC_disalign_LL(void);

/**********************************************/
/* function serial_cmd_handle()               */
/* Gestisce i comandi ricevuti sulla seriale  */
/**********************************************/
BOOL serial_ESC_debug(char c)
  {
    switch (c)
    {
        case RTC_DISAL:
            RTC_disalign_LL();
            break;
        case CONFIG_PRINT:
            break;
        
        case TEST_PRINT:
            break;

        case DEBUG_ENABLE:
            debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_all_dbg_ON));
            rtc_dbg = true;
            sd_dbg  = true;
            lcd_dbg = true;
            eth_dbg = true;
            srv_dbg = true;
            mot_dbg = true;
            lgc_dbg = true;
            plc_dbg = true;
            eep_dbg = true;
            bdg_dbg = true;
            ser_dbg = true;
            io_dbg  = true;
            fsm_dbg = true;
            erg_dbg = true;
            break;

        case DEBUG_DISABLE:
            debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_all_dbg_OFF));
            rtc_dbg = false;
            sd_dbg  = false;
            lcd_dbg = false;
            eth_dbg = false;
            srv_dbg = false;
            mot_dbg = false;
            lgc_dbg = false;
            plc_dbg = false;
            eep_dbg = false;
            bdg_dbg = false;
            ser_dbg = false;
            io_dbg  = false;
            fsm_dbg = false;
            erg_dbg = false;
            break;
        case RTC_DBG:
            if (rtc_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_RTC));
                rtc_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_RTC));
                rtc_dbg = true;
            }
            break;

        case SD_DBG:
            if (sd_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_SD));
                sd_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_SD));
                sd_dbg = true;
            }
            break;
        
        case LCD_DBG:
            if (lcd_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_LCD));
                lcd_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_LCD));
                lcd_dbg = true;
            }
            break;
        
        case ETH_DBG:
            if (eth_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_ETH));
                eth_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_ETH));
                eth_dbg = true;
            }
            break;
        
        case SRV_DBG:
            if (srv_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_SRV));
                srv_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_SRV));
                srv_dbg = true;
            }
            break;
        
        case MOT_DBG:
            if (mot_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_MOT));
                mot_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_MOT));
                mot_dbg = true;
            }
            break;
        
        case LGC_DBG:
            if (lgc_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_LOG));
                lgc_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_LOG));
                lgc_dbg = true;
            }
            break;
        
        case PLC_DBG:
            if (plc_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_PLC));
                plc_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_PLC));
                plc_dbg = true;
            }
            break;
        
        case EEP_DBG:
            if (eep_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_EEP));
                eep_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_EEP));
                eep_dbg = true;
            }
            break;
        
        case BDG_DBG:
            if (bdg_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_BDG));
                bdg_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_BDG));
                bdg_dbg = true;
            }
            break;
        
        case SER_DBG:
            if (ser_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_SER));
                ser_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_SER));
                ser_dbg = true;
            }
            break;

        case IO_DBG:
            if (io_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_GPIO));
                io_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_GPIO));
                io_dbg = true;
            }
            break;

        case INFO_DBG:
            appl_init_title();
            break;

        case FSM_DBG:
            if (fsm_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_FSM));
                fsm_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_FSM));
                fsm_dbg = true;
            }
            break;

        case EROG_DBG:
            if (erg_dbg)
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_disena_EROG));
                erg_dbg = false;
            }
            else
            {
                debug_print_timestamp_ident(DBG_ALWAYS_ON, DEBUG_IDENT_L3, AVR_PGM_to_str(str_dbg_ena_EROG));
                erg_dbg = true;
            }
            break;

        default:
            ;
    }
    Serial.flush();
     return TRUE;
  }

/**********************************************/
/* void serial_debug_init(void)               */
/*--------------------------------------------*/
/* Inizializzazione ad alto livello dei flag  */
/* per la gestione del debug selettivo.       */
/**********************************************/
void serial_debug_init(void)
{
    rtc_dbg = false;
    sd_dbg  = false;
    lcd_dbg = false;
    eth_dbg = true;
    srv_dbg = true;
    mot_dbg = false;
    lgc_dbg = false;
    plc_dbg = false;
    eep_dbg = false;
    bdg_dbg = false;
    ser_dbg = true;
    io_dbg  = false;
    fsm_dbg = true;
    erg_dbg = true;
}

#undef MODULE_SERIAL_COMMAND_DEBUG
