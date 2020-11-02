// include specifici per le librerie LANZI

// include standard del "C"
#include "stdint.h"
#include "string.h"

// definizioni e abilitazioni globali
#include "project_def.h"
#include "debug_opzion.h"

// include del core di Arduino
#include "Arduino.h"
#include <Wire.h>

// include specifici Lanzi
#include "./Include/lanzi_def.h"
#include "./Include/player_def.h"

// include librerie generalizzate
#include "../GP_Library/Include/mydef.h"
#include "../GP_Library/Include/my_time.h"
#include "../GP_Library/Include/global_define.h"
#include "../GP_Library/Debug/Utility/debug_utility.h"
#include "../GP_Library/Command/serial_command.h"
#include "../GP_Library/Command/serial_command_debug.h"
#include "../GP_Library/FSM/FSM_template.h"
#include "../GP_Library/FSM/FSM_Tiny_template.h"
#include "../GP_Library/RTC/RTC_template.h"
#include "../GP_Library/Miscell/AVR/AVR_API.h"
#include "../GP_Library/Miscell/CRC_checksum/CRC_func.h"

// include libreria hardware
#include "../Hardware/CPU_core/timing.h"
#include "../Hardware/Badge/gestione_badge.h"

// include in cartella "Common"
#include "./Common/Lanzi_tag.h"

// include in cartella "Command"
#include "./Command/serial_command_LANZI.h"

// include in cartella "Storage"
#include "./Storage/storage_def.h"
#include "./Storage/storage_main.h"
#include "./Storage/storage_print.h"

// include in cartella "Anagrafica"
#include "./Anagrafica/anag_def.h"
#include "./Anagrafica/anag_lib.h"
#include "./Anagrafica/anag_read.h"
#include "./Anagrafica/anag_write.h"
#include "./Anagrafica/anag_logicFSM.h"
#include "./Anagrafica/anag_API.h"

// include in cartella "Badge"
#include "./Badge/badge_lib.h"

// include in cartella "Config"
#include "./Config/config_def.h"
#include "./Config/config_lib.h"
#include "./Config/config_utils.h"
#include "./Config/config_logicFSM.h"
#include "./Config/config_decode.h"
#include "./Config/config_API.h"

// include in cartella "Offline"

// include in cartella "Server"
#include "./Server/server_def.h"
#include "./Server/server_lib.h"
#include "./Server/server_utility.h"

// include in cartella "Virtual_Server"

