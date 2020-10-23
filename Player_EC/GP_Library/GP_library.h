// include specifici per le libreria GP_library

// include standard del "C"
#include "stdint.h"
#include "string.h"

// definizioni e abilitazioni globali
#include "project_def.h"
#include <debug_opzion.h>

// include del core di Arduino
#include "Arduino.h"
#include <Wire.h>

// include librerie generalizzate
#include ".//Include/mydef.h"
#include "./Include/global_define.h"
#include "./Include/my_time.h"
#include "./Miscell/Utility/print_utility.h"
#include "./Miscell/AVR/AVR_API.h"

// include in cartella "Debug"
#include "./Debug/Utility/debug_utility.h"

// include in cartella "Command"
#include "./Command/serial_command.h"
#include "./Command/serial_command_debug.h"

// include in cartella "EEPROM"
#include "./EEPROM/EEPROM_template.h"

// include in cartella "Ethernet"
#include "./Ethernet/Ethernet_template.h"

// include in cartella "FSM"
#include "./FSM/FSM_template.h"
#include "./FSM/FSM_Tiny_template.h"

// include in cartella "LCD"
#include "./LCD/LCD_template.h"

// include in cartella "PLC"
#include "./PLC/PLC_define.h"

// include in cartella "RTC"
#include "./RTC/RTC_template.h"

// include in cartella "SD"
#include "./SD/SD_template.h"

// include esterni in libreria Hardware
#include "../Hardware/CPU_core/timing.h"
#include "../Hardware/Display/gestione_LCD.h"
#include "../Hardware/RTC/gestione_RTC.h"
#include "../Hardware/Debug/debug_serial.h"