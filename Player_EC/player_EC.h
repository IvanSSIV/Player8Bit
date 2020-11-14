/****************************************************************************/
/* File: player_EC.h                                                        */
/* Data creazione: 17/03/202                                                */
/* Creato da: Adriano Olivetti - Ivan De Stefani	                    */
/* Funzione: File header di inclusione delle librerie di progetto  	    */
/*                                                                          */
/* Changelog:                                                               */
/*                                                                          */
/*                                                                          */
/****************************************************************************/


// prova gitub
// include standard "C" e Arduino
#include "Arduino.h"

// librerie personalizzate
#include "GP_library/Include/mydef.h"
#include "GP_library/Include/my_time.h"
#include "GP_library/Include/global_define.h"

// include specifici Lanzi
#include "./Lanzi_Library/Include/lanzi_def.h"
#include "./Lanzi_Library/Include/player_def.h"

// include a livello di root del progetto 
#include "debug_opzion.h"
#include "project_def.h"

// utility per debug
#include "GP_Library/Debug/Utility/debug_utility.h"
#include "GP_library/Debug/Gestione_Errori/gestione_errori.h"
#include "Hardware/Debug/debug_serial.h"
#include "Hardware/Debug/debug_HW.h"
#include "GP_Library/Command/serial_command.h"
#include "GP_Library/Command/serial_command_debug.h"
//#include "GP_Library/Debug/Serial_Command/serial_command.h"

// codice libreria generiche
#include "GP_library/Miscell/AVR/AVR_api.h"
#include "GP_library/FSM/FSM_template.h"
#include "GP_library/PLC/PLC_define.h"
#include "GP_library/LCD/LCD_template.h"
#include "GP_library/RTC/RTC_template.h"
#include "GP_library/SD/SD_template.h"
// Da verificare
#include "GP_Library/Ethernet/Ethernet_template.h"
#include "GP_Library/Ethernet/PLC_server_interface.h"

#include "GP_Library/EEPROM/EEPROM_template.h"
#include "GP_Library/FSM/FSM_Tiny_template.h"
#include "GP_Library/Miscell/CRC_checksum/CRC_func.h"
#include "GP_Library/Miscell/Utility/print_utility.h"

// codice hardware dipedent
#include "Hardware/Seriali/serial_init.h"
#include "Hardware/CPU_core/timing.h"
#include "Hardware/PLC_IO/gestione_io.h"
#include "Hardware/Display/gestione_LCD.h"
#include "Hardware/RTC/gestione_RTC.h"
#include "Hardware/Motore/gestione_motore_nodenti.h"
#include "Hardware/Badge/gestione_badge.h"
//#include "Hardware/Special/hi_speed_input.h"

//Librerie Lanzi
#include "Lanzi_Library/Config/config_def.h"
#include "Lanzi_Library/Config/config_lib.h"
#include "Lanzi_Library/Config/config_utils.h"
#include "Lanzi_Library/Config/config_read.h"
#include "Lanzi_Library/Config/config_logicFSM.h"
#include "Lanzi_Library/Anagrafica/anag_logicFSM.h"
#include "Lanzi_Library/Badge/badge_lib.h"
#include "Lanzi_Library/Server/server_def.h"
#include "Lanzi_Library/Server/server_lib.h"
#include "Lanzi_Library/Server/server_utility.h"
#include "Lanzi_Library/Storage/storage_def.h"
#include "Lanzi_Library/Storage/storage_main.h"
#include "Lanzi_Library/Storage/storage_print.h"


// codice di applicazione
//#include "Application_Code/IO_mapping.h"
#include "Application_Code/Init/appl_init.h"
#include "Application_Code/Main/appl_main.h"
#include "Application_Code/Main/global_var.h"
#include "Application_Code/Logica/appl_logica.h"

// include per codice specifico di uno specifico player
#if MACHINE_TYPE == ARGO_EC
#include "Application_Code/Specific_code/EC/Erog/appl_erog_EC.h"
#include "Application_Code/Specific_code/EC/IO_mapping.h"
#include "Application_Code/Specific_code/EC/Special_func/hi_speed_input.h"
#include "Application_Code/Specific_code/EC/Motor/appl_motor.h"

#elif MACHINE_TYPE == ARGO_KD
#include "Application_Code/Erog/KD/appl_erog_KD.h"
#elif MACHINE_TYPE == ARGO_LK
#include "Application_Code/Specific_code/LK/Erog/appl_erog_LK.h"
#elif MACHINE_TYPE == ARGO_GR
#include "Application_Code/Specific_code/GR/GR_IO_mapping.h"
#include "Application_Code/Specific_code/GR/Erog/appl_erog_GR.h"
#include "Application_Code/Specific_code/GR/Motor/appl_motor_GR.h"
#elif MACHINE_TYPE == ARGO_XL
#include "Application_Code/Erog/KD/appl_erog_XL.h"
#endif

#include "Application_Code/Test/test_code_init.h"
#include "Application_Code/Test/test_code_main.h"
#include "Application_Code/Test/serial_command_test.h"