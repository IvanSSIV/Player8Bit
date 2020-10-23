/****************************************************************************/
/* File: gestione_LCD.cpp                                                   */
/* Data creazione: 24/03/2020                								*/
/* Creato da: Olivetti - De Stefani      									*/
/* Funzione: Driver del modulo LCD 4x20.                           			*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/
#include "..\Lanzi_8bit_2560.h"
#include <LiquidCrystal.h>
#include "gestione_LCD.h"

/**************************************************/
/* La prima riga sarà "indipendente" dalle altre, */
/* e avrà un messaggio fisso che comprende anche  */
/* l'ora.                                         */
/**************************************************/


// Stringhe in program memory per stampa di debug
static const char str_lcd_init_start[] PROGMEM = { "- Inizializzo interfaccia LCD -" };
static const char str_lcd_init_end[] PROGMEM   = { "- Fine inizializzazione LCD -"};

// Definizione dell'istanza della classe LiquidCrystal
LiquidCrystal lcd(8, 7, 6, 45, 44, 43, 42);

// prototipi funzione locali
static void my_LCD_printXY(char* message, uint8_t x, uint8_t y);

/********************************************/
/* void LCD_init(void)                      */
/*------------------------------------------*/
/* Funzione di inizializzazione del display */
/* LCD 4x20 tramite la libreria             */
/* "LiquidCrystal" di Arduino.              */
/********************************************/
void LCD_init(void)
  {
     debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_lcd_init_start);

     lcd.begin(20,4);
     lcd.print("Hello world");

     debug_message_timestamp_PGM(str_lcd_init_end);
  }


/********************************************/
/* void LCD_write_linen(char * message)     */
/*------------------------------------------*/
/* Funzioni per la scrittura sulle varie    */
/* righe del display LCD.                   */
/********************************************/
void LCD_write_line1(char *message)
  {
     my_LCD_printXY(message, 0, 0);
  }
void LCD_write_line2(char* message)
  {
     my_LCD_printXY(message, 0, 1);
  }
void LCD_write_line3(char* message)
  {
     my_LCD_printXY(message, 0, 2);
  }
void LCD_write_line4(char* message)
  {
     my_LCD_printXY(message, 0, 3);
  }

/********************************************************************/
/* static void my_LCD_printXY(char * message, uint8_t x, uint8_t y) */
/*------------------------------------------------------------------*/
/* Scrive un messaggio alle coordinate (x, y) del display LCD.      */
/********************************************************************/
static void my_LCD_printXY(char* message, uint8_t x, uint8_t y)
{
    char temp[24];
    uint8_t size;

    memset(temp, 0x20, sizeof(temp));
    temp[LCD_MAX_SIZE] = 0;
    
    size = strlen(message) < LCD_MAX_SIZE ? strlen(message) : LCD_MAX_SIZE;
    memcpy(temp, message, size);
    
    lcd.setCursor(x, y);
    lcd.print(message);
    //lcd.print(temp);

}