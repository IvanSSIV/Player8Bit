#ifdef MODULE_CRC_FUNC
#define _EXTVAR 
#else
#define _EXTVAR extern
#endif

#ifdef CRC_FAST
_EXTVAR BYTE ucCRCHi;
_EXTVAR BYTE ucCRCLo;
_EXTVAR WORD CRC_iIndex;
#endif

#ifdef CRC_SMALL
_EXTVAR BYTE CRC_bit;
_EXTVAR WORD usCRC;
_EXTVAR WORD CRC_flag;
#endif

WORD calcola_CRC_CCITT(void* buf, WORD len);
WORD calcola_CRC_MDB(void* addr, WORD len);


#undef _EXTVAR
