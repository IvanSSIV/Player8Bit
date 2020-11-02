#include "stdint.h"
#include "Arduino.h"
#include "debug_serial.h"

void debug_serial_init(void)
  {
     Serial.begin(115200L);
  }

/*--------------------------------------------------------------------*/
/* function debug_char_available()                                    */
/* Verifica se sono presenti dei caratteri nel buffer di ricezione    */
/* ed esce immediatamente. In uscita restituisce TRUE se vi sono dei  */
/* caratteri o FALSE in caso contrario                                */
/*--------------------------------------------------------------------*/
uint8_t debug_char_available(void)
{
    return Serial.available();
}

/*---------------------------------------------------*/
/* function debug_read_char()                        */
/* legge un carattere dalla seriale                  */
/*---------------------------------------------------*/
uint8_t debug_read_char(void)
{
    return Serial.read();
}

/*---------------------------------------------------*/
/* functiom debug_write_char()                       */
/* scrive un carattere sulla seriale                 */
/*---------------------------------------------------*/
void debug_write_char(char dato)
{
    Serial.write(dato);
}

/*---------------------------------------------------*/
/* scrive una stringa di testo sulla seriale         */
/*---------------------------------------------------*/
void debug_print(char* message)
{
    Serial.print(message);
}
