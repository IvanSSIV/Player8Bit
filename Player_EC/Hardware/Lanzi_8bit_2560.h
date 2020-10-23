// include specifici per la scheda LANZI a 8bit

// include standard del "C"
#include "stdint.h"
#include "string.h"

// definizioni e abilitazioni globali
#include "project_def.h"
#include <debug_opzion.h>

// include del core di Arduino
#include "Arduino.h"
#include <Wire.h>

#include "../GP_Library/Include/mydef.h"
#include "../GP_Library/Include/my_time.h"
#include "../GP_Library/Debug/Utility/debug_utility.h"
#include "../GP_Library/Command/serial_command.h"
#include "../GP_Library/PLC/PLC_define.h"
#include "../GP_Library/Miscell/AVR/AVR_API.h"

#include "./CPU_core/timing.h"
