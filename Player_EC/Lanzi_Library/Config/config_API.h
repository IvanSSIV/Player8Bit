// prototipi di funzione globali
BOOL config_openwr(void);
void config_closewr(void);
void config_writeline(char* line);

void config_openrd(char *filename);
BOOL config_file_exist(char *filename);
int config_char_available(void);
int config_read_char(void);
void config_closerd(void);
