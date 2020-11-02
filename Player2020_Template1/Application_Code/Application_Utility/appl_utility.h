#ifdef MODULE_APPL_UTILITY
#define _EXTVAR /**/
#else
#define _EXTVAR extern
#endif 




struct ip_addr
{
    uint8_t oct1;
    uint8_t oct2;
    uint8_t oct3;
    uint8_t oct4;
};
typedef struct ip_addr MY_IP_ADDR;

void appl_init_title(void);

#undef _EXTVAR