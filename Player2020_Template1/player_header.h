/****************************************************************************/
/* File: player_header.h                                                        */
/* Data creazione: 17/03/2020  												*/
/* Creato da: Adriano Olivetti - Ivan De Stefani							*/
/* Funzione: File header di inclusione delle librerie di progetto  			*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

// include standard "C" e Arduino
#include "Arduino.h"
// librerie personalizzate
#include "My_Include/mydef.h"
#include "My_Include/my_time.h"
#include "My_Include/global_define.h"
// utility varie 
#include "debug_opzion.h"
#include "project_def.h"
#include "IO_mapping.h"
// utility per debug
#include "My_debug/Debug_Serial/debug_serial.h"
#include "My_debug/Debug_Utility/debug_utility.h"
#include "My_Debug/Debug_HW/debug_HW.h"
#include "My_Debug/Serial_Command/serial_command.h"
// codice libreria generiche
#include "My_Library/Utilities/AVR_API.h"
#include "My_Library/FSM/FSM_template.h"
#include "My_Library/PLC/PLC_define.h"
#include "My_Library/LCD/LCD_template.h"
#include "My_Library/RTC/RTC_template.h"
#include "My_Library/SD/SD_template.h"
#include "My_Library/Connessione/Ethernet_template.h"
#include "My_Library/EEPROM/EEPROM_template.h"


// codice hardware dipedent
#include "Hardware/Gestione_seriali/serial_init.h"
#include "Hardware/CPU_core/timing.h"
#include "Hardware/Gestione_IO/gestione_io.h"
#include "Hardware/Gestione_display/gestione_LCD.h"
#include "Hardware/Gestione_RTC/gestione_RTC.h"
#include "Hardware/Gestione_motore/gestione_motore_nodenti.h"
#include "Hardware/Badge/gestione_badge.h"
#include "Hardware/Special/hi_speed_input.h"

// codice di applicazione
#include "Application_Code/Application_Init/appl_init.h"
#include "Application_Code/Application_Main/appl_main.h"
#include "Application_Code/Application_Utility/appl_utility.h"
#include "Application_Code/Application_Test/appl_test_PLC.h"
#include "Application_Code/Application_Server/appl_server.h"
#include "Application_Code/Application_Server/server_utility.h"
#include "Application_Code/Application_Motor/appl_motor.h"
#include "Application_Code/Application_Erog/appl_badge.h"
#include "Application_Code/Application_Erog/appl_erog.h"
#include "Application_Code/Application_Logica/appl_logica.h"
#include "Application_Code/Application_Config/appl_config.h"
#include "Application_Code/Application_Config/config_utils.h"