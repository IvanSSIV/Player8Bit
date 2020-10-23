// prototipi funzione
void debug_print_ena(BOOL ena, char* message);
void debug_print_CRLF(BOOL ena);
void debug_print_timestamp(BOOL ena, char* msg);
void debug_print_timestamp_rel(BOOL ena, char* msg, DWORD ts);
void debug_print_timestamp_ident(BOOL ena, BYTE ident, char* msg);
void debug_printbuff_hex(BOOL ena, BYTE* buff, BYTE buff_size);
void debug_print_timestamp_title(BOOL ena,uint8_t ident, char* title, char* msg);
void debug_make_buff_hex(char* buff, uint8_t size, uint8_t* data, uint8_t data_size);

void debug_message_init(void);
void debug_message(char *message);
void debug_message_PGM(const char *message);
void debug_message_timestamp(char* message);
void debug_message_timestamp_PGM(const char* message);
void debug_message_CRLF(void);
void debug_message_enable(BOOL ena,uint8_t ident);
void debug_message_disable(void);
void debug_message_set_ident(uint8_t ident);
void debug_message_ident_push(void);
void debug_message_ident_pop(void);
void debug_message_tag_ident(const char *tag,const char *message);
void debug_message_tag_ident_PGM(const char *tag,const char *message);