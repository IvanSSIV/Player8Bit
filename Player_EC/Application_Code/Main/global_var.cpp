#define MODULE_GLOBAL_VAR
#include "player_EC.h"

void global_data_init(void)
  {
     memset(&global_data, 0, sizeof(global_data));
     global_data.flg_boot = TRUE;
#if MACHINE_TYPE == ARGO_EC
     global_data.machine_id = ARGO_EC;
#elif MACHINE_TYPE == ARGO_KD
     global_data.machine_id = ARGO_KD;
#elif MACHINE_TYPE == ARGO_LK
     global_data.machine_id = ARGO_LK;
#elif MACHINE_TYPE == ARGO_GR
     global_data.machine_id = ARGO_GR;
#elif MACHINE_TYPE == ARGO_XL
     global_data.machine_id = ARGO_XL;
#endif
  }


#undef MODULE_GLOBAL_VAR
