#include "Arduino.h"
/*******************************************/
/* void serial_init(void)                  */
/*-----------------------------------------*/
/* Inizializza la seriale su MCP2200.      */
/*******************************************/
void serial_init(void)
{
    // inizializza seriale di debug
    Serial.begin(115200L);
}