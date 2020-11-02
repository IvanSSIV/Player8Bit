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
/**************************************************/
/* La prima riga sarà "indipendente" dalle altre, */
/* e avrà un messaggio fisso che comprende anche  */
/* l'ora.                                         */
/**************************************************/
#include "Arduino.h"
#include "project_def.h"
//#include <Application_Code/project_def.h>
#include <debug_opzion.h>
#include <My_Include/mydef.h>
#include <My_Debug/Debug_Serial/debug_serial.h>
#include <My_Debug/Debug_Utility/debug_utility.h>
#include "gestione_LCD.h"
#include <My_Debug/Serial_Command/serial_command.h>

// Include della libreria LCD di Arduino
#include <LiquidCrystal/LiquidCrystal.h>

// Definizione dell'istanza della classe LiquidCrystal
LiquidCrystal lcd(8, 7, 6, 45, 44, 43, 42);

// prototipi funzione locali
static void my_LCD_printXY(char* message, uint8_t x, uint8_t y);


void LCD_init(void)
  {
     debug_print_timestamp(DBG_ALWAYS_ON, (char*)"inizializzo interfaccia LCD ..");
     lcd.begin(20,4);
     lcd.print("Hello world");
     debug_print_timestamp(DBG_ALWAYS_ON, (char*)"fine inizializzazione LCD");
  }



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

// visualizza un messaggio su una riga fillata con blank
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