

// prototipi funzione
void leggi_data_ora_da_RTC(void);
void RTC_ena_print(void);
void RTC_dis_print(void);
void get_datetime_ASCII(char* date_time, uint8_t date_size);
void get_datetime_ASCII_filedat(char* date_time, uint8_t date_size);
void get_datetime_ASCII_Lanzi(char* date_time, uint8_t date_size);
BOOL set_data_ora_da_server(char* data_ora_server);
void RTC_print(RTC_WORK* dataora);