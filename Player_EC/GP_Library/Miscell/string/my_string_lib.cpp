#include "string.h"
#include "stdint.h"
#include "../../include/mydef.h"

/*--------------------------------*/
/* function my_strncat()          */
/*--------------------------------*/
void my_strncat(void *dest,void *source,uint16_t size)
  {
     strncat((char *)dest,(char *)source,size - strlen((char*)dest) - 1);
  }
