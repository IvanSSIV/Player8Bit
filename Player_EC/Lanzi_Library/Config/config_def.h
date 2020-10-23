
// definizione supporto per salvataggio
#define CONFIG_MEDIA MEDIA_SD

// struttura dati di un singolo settore
struct sect_data
{
    uint8_t  piatto;     // piatto a cui appartiene il settore
    uint8_t  pos_start;  // posizione inizio (numero dente)
    uint8_t  pos_end;    // posizione fine   (numero dente)
    uint8_t  n_celle;    // numero di celle nel settore
};
typedef struct sect_data SEC_DATA;

// struttura paramentri LAN
struct config_LAN
{
    MY_IP_ADDR  IP_addr;                            // IP macchina
    MY_IP_ADDR  SM_addr;                            // subnet mask
    MY_IP_ADDR  GW_addr;                            // gateway
    MY_IP_ADDR  DNS_addr;                           // DNS
    uint8_t     MAC[6];                                // MAC address
    MY_IP_ADDR  SERV_addr;                          // IP server
    uint16_t  portnum;                               // numero porta        
};
typedef struct config_LAN CONF_LAN;

// struttura parametri macchina
struct config_MACHINE
{
    uint16_t  clock_update_tim; // tempo per lettura data-ora da server
    uint16_t  prod_select_tim;  // timeout selezione prodotto
    uint16_t  offline_timeout;
    uint16_t  badge_start;      // carattere inizio codice badge
    uint16_t  badge_end;        // carattere fine codice badge
    uint16_t  auto_jump;        // flag abilitazione autojump
    uint16_t  LED_standby;      // tempo stand-by LED
    uint16_t  livello_1;
    uint16_t  livello_2;
    uint16_t  ritardo_taglio_motore;
    uint16_t  taglio_motore;
    uint16_t  ritardo_stop_motore;
    uint16_t  look_door_tim;    // tempo attivazione elettroserrature
    uint16_t  param14;
    uint16_t  param15;
    uint16_t  param16;
};
typedef struct config_MACHINE CONF_MACH;

// struttura complessiva del config
#define CONFIG_FILL_SIZE 18   // dimensione eventuale zona riempimento
struct config_dat_mem
{
    uint16_t  marca_ini;                             // marca di inizio struttura
    uint16_t  CRC;                                   // CRC della struttura (da CRC+2 a fine struttura)

    // dati identificazione macchina
    char      computer_num[MAX_SIZE_NUM_COMPUTER];   // numero computer    
    char      computer_name[MAX_SIZE_NOME_COMPUTER]; // nome computer

    // dati LAN
    CONF_LAN  dati_LAN;                              // dati LAN

    // parametri macchina
    CONF_MACH dati_macchina;                         // dati macchina    

    // descrittore settori
    uint16_t  num_settori;
    SEC_DATA  dati_settori[MAX_SETTORI];             // dati settori

    UINT8     boot_mode;
    // riempimento
    uint8_t   fill[CONFIG_FILL_SIZE];                // riempimento per avere multipli del settore EEPROM

    uint16_t  marca_end;                             // marca di fine struttura
};
typedef struct config_dat_mem CONFIG_DAT;

// struttura dati di lavoro per gestione config
struct config_work_data
  {
     BOOL     dbg_ena;          // abilitazione debug modulo
     uint8_t  dbg_idn;          // livello identazione modulo
     TINY_FSM LAN_FSM_read;     // FSM gestione lettura da LAN
     uint8_t  LAN_retry;        // conteggio retry per server ready
     uint8_t  LAN_error_reason; // ragione della condizion di errore
     BOOL     LAN_read_start;   // flag avvio lettura
     TINY_FSM SD_FSM_read;      // FSM gestione lettura da SD
     uint8_t  SD_retry;         // conteggio retry per server ready
     uint8_t  SD_error_reason;  // ragione della condizion di errore
     BOOL     SD_read_start;    // flag avvio lettura

  };
typedef struct config_work_data CONFIG_WORK;
