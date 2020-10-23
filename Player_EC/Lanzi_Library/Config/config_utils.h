
void config_decode_line_from_server(char* read_line, BOOL print_ena);
void config_decode_line_from_SD(char *read_line,BOOL print_ena);
void config_display_all(void);
void config_init_data(void);
BOOL config_verify(void);
void config_update_CRC(void);
void config_test(void);
void config_debug_print(char *message);
void config_debug_print_PGM(const char *message);
