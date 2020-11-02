// definizioni globali
#define LCD_EVENT_CLEAR_ALL    1 // cancello tutto il display
#define LCD_EVENT_LINE1_STOP   2 // fermo gestione riga 1
#define LCD_EVENT_LINE1_START  3 // avvio gestione riga 1
#define LCD_EVENT_UPDATE_LINE1 4 // richiesta scrittura riga 1
#define LCD_EVENT_UPDATE_LINE2 5 // richiesta scrittura riga 2
#define LCD_EVENT_UPDATE_LINE3 6 // richiesta scrittura riga 3
#define LCD_EVENT_UPDATE_LINE4 7 // richiesta scrittura riga 4


void LCD_hand(void);
void LCD_line1_update(char* buffer);
void LCD_send_event(uint8_t event, char* message);