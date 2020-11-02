// prototipi funzione
void debug_print_ena(BOOL ena, char* message);
void debug_print_CRLF(BOOL ena);
void debug_print_timestamp(BOOL ena, char* msg);
void debug_print_timestamp_rel(BOOL ena, char* msg, DWORD ts);
void debug_print_timestamp_ident(BOOL ena, BYTE ident, char* msg);
void debug_printbuff_hex(BOOL ena, BYTE* buff, BYTE buff_size);
void debug_print_timestamp_title(BOOL ena,uint8_t ident, char* title, char* msg);
