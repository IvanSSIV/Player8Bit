/*----------------------------------------------------------------*/
/* definisce enumerazioni per i vari player e le possibili schede */
/*----------------------------------------------------------------*/

// definizione modelli di player
#define ARGO_EC 1
#define ARGO_GR 2
#define ARGO_LK 3
#define ARGO_KD 4
#define ARGO_XL 5


// enum per definzione piattaforme hardware
enum lanzi_hardware_platform
  {
     LANZI_8BIT_2560,
     LANZI_32BIT_SAM3X8
  };
typedef enum lanzi_hardware_platform LANZI_HW_ID;
