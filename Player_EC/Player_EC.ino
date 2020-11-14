/****************************************************************************/
/* File: Player_EC.ino                                                      */
/* Data creazione: 17/03/2020  												*/
/* Creato da: Adriano Olivetti - Ivan De Stefani                        	*/
/* Funzione: File "base" della struttura di Arduino                			*/
/*																			*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/
#include <SoftwareSerial.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <SPI.h>
#include <SD.h>
#include <utility/SdFat.h>
#include <utility/SdFatUtil.h>
#include <Ethernet.h>
#include <utility/w5100.h>
#include <EEPROM.h>


extern void appl_init(void);
extern void appl_main(void);

// the setup function runs once when you press reset or power the board
void setup() {
    appl_init();        // Inizializzazione dei moduli
}

// the loop function runs over and over again until power down or reset
void loop() {
    appl_main();        // Main dell'applicazione astratto per porting su altri dispositivi
}
