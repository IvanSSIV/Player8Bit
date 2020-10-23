/****************************************************************************/
/* File: gestione_io.cpp                                                    */
/* Data creazione: 24/03/2020  												*/
/* Creato da: Adriano Olivetti - De Stefani Ivan							*/
/* Funzione: Gestisce gli I/O digitali "PLC-like" ad eccezione di: 			*/
/*				-motore, encoder    										*/
/*              -I/O speciali su interrupt                                  */
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/
#include "..\Lanzi_8bit_2560.h"
#include "gestione_io.h"
#include "GPIO_define.h"

// Stringhe in program memory per messaggi di debug
const char str_gpio_init_start[] PROGMEM = { "- Inizializzo GPIO -" };
const char str_gpio_init_end[] PROGMEM   = { "- Fine inizializzazione GPIO! -" };

/*-----------------------------------------------------------*/
/* function GPIO_init()                                      */
/* Inizializza mediante primitive HW independent i registri  */
/* DDR delle porte del micro.                                */
/*-----------------------------------------------------------*/
void GPIO_init(void)
  {
     debug_message_enable(TRUE,2);
     debug_message_CRLF();
     debug_message_timestamp_PGM(str_gpio_init_start);

     #ifdef GPIO_ARDUINO_MODE
        pinMode(GPIO_IN_1, INPUT);
        pinMode(GPIO_IN_2, INPUT);
        pinMode(GPIO_IN_3, INPUT);
        pinMode(GPIO_IN_4, INPUT);
        pinMode(GPIO_IN_5, INPUT);
        pinMode(GPIO_IN_6, INPUT);
        pinMode(GPIO_IN_7, INPUT);
        pinMode(GPIO_IN_8, INPUT);
        pinMode(GPIO_IN_9, INPUT);
        pinMode(GPIO_IN_10, INPUT);
        pinMode(GPIO_IN_11, INPUT);
        pinMode(GPIO_IN_12, INPUT);
        pinMode(GPIO_IN_13, INPUT);
        pinMode(GPIO_IN_14, INPUT);
        pinMode(GPIO_IN_15, INPUT);
        pinMode(GPIO_IN_16, INPUT);
        pinMode(GPIO_IN_17, INPUT);
        pinMode(GPIO_IN_18, INPUT);
        pinMode(GPIO_IN_19, INPUT);
        pinMode(GPIO_IN_20, INPUT);
        pinMode(GPIO_IN_21, INPUT);
        pinMode(GPIO_IN_22, INPUT);
        pinMode(GPIO_IN_23, INPUT);
        pinMode(GPIO_IN_FORCELLA, INPUT);

        pinMode(GPIO_OUT_LED, OUTPUT);
        digitalWrite(GPIO_OUT_LED, LOW);

        pinMode(GPIO_OUT_1, OUTPUT);
        digitalWrite(GPIO_OUT_1, LOW);

        pinMode(GPIO_OUT_2, OUTPUT);
        digitalWrite(GPIO_OUT_2, LOW);

        pinMode(GPIO_OUT_3, OUTPUT);
        digitalWrite(GPIO_OUT_3, LOW);

        pinMode(GPIO_OUT_4, OUTPUT);
        digitalWrite(GPIO_OUT_4, LOW);

        pinMode(GPIO_OUT_5, OUTPUT);
        digitalWrite(GPIO_OUT_5, LOW);

        pinMode(GPIO_OUT_6, OUTPUT);
        digitalWrite(GPIO_OUT_6, LOW);

        pinMode(GPIO_OUT_7, OUTPUT);
        digitalWrite(GPIO_OUT_7, LOW);

        pinMode(GPIO_OUT_8, OUTPUT);
        digitalWrite(GPIO_OUT_8, LOW);

        pinMode(GPIO_OUT_9, OUTPUT);
        digitalWrite(GPIO_OUT_9, LOW);

        pinMode(GPIO_OUT_10, OUTPUT);
        digitalWrite(GPIO_OUT_10, LOW);

        pinMode(GPIO_OUT_11, OUTPUT);
        digitalWrite(GPIO_OUT_11, LOW);

        pinMode(GPIO_OUT_12, OUTPUT);
        digitalWrite(GPIO_OUT_12, LOW);

        pinMode(GPIO_OUT_13, OUTPUT);
        digitalWrite(GPIO_OUT_13, LOW);

        pinMode(GPIO_OUT_14, OUTPUT);
        digitalWrite(GPIO_OUT_14, LOW);

        pinMode(GPIO_OUT_15, OUTPUT);
        digitalWrite(GPIO_OUT_15, LOW);

        pinMode(GPIO_OUT_16, OUTPUT);
        digitalWrite(GPIO_OUT_16, LOW);

        pinMode(GPIO_OUT_17, OUTPUT);
        digitalWrite(GPIO_OUT_17, LOW);

        pinMode(GPIO_OUT_18, OUTPUT);
        digitalWrite(GPIO_OUT_18, LOW);

        pinMode(GPIO_OUT_19, OUTPUT);
        digitalWrite(GPIO_OUT_19, LOW);

        pinMode(GPIO_OUT_20, OUTPUT);
        digitalWrite(GPIO_OUT_20, LOW);

        pinMode(GPIO_OUT_21, OUTPUT);
        digitalWrite(GPIO_OUT_21, LOW);

        pinMode(GPIO_OUT_22, OUTPUT);
        digitalWrite(GPIO_OUT_22, LOW);
     #endif

     #ifdef GPIO_PORT_MODE
        uint8_t temp;
        cli();

        //ingressi
        temp = DDRF;
        temp &= 0x01;                          //0000 0001
        DDRF = temp;    // in 1-7, PF1-PF7

        DDRK = 0x00;    // in 8-15
        DDRA = 0x00;    // in 16-23
        temp = DDRG;
        temp &= 0xDF;
        DDRG = temp;    // in forcella, PG5      1101 1111

        //uscite
        temp = DDRB;    
        temp |= 0x40;   // 0100 0000
        DDRB = temp;    // out LED

        DDRC = 0xFF;    // out 6-13

        temp = DDRD;
        temp |= 0x03;
        DDRD = temp;    // out 19-20

        temp = DDRE;
        temp |= 0x10;
        DDRE = temp;    // out 18

        temp = DDRG;
        temp |= 0x1F;
        DDRG = temp;    // out 2, 3, 4, 5, 16, 21

        temp = DDRH;
        temp |= 0xC0;
        DDRH = temp;    // out 1, 17

        temp = DDRJ;
        temp |= 0x83;
        DDRJ = temp;    // out 14, 15, 22

        // metto tutte le uscite a zero
        PORTB = PORTB & 0xBF;       // 1011 1111
        PORTH = PORTH & 0x7F;       // 0111 1111
        PORTG = PORTG & 0xF7;       // 1111 0111
        PORTG = PORTG & 0xEF;       // 1110 1111
        PORTG = PORTG & 0xFE;       // 1111 1110
        PORTG = PORTG & 0xFD;       // 1111 1101
        PORTC = PORTC & 0x00;       // 0000 0000
        PORTC = PORTC & 0x7F;       // 0111 1111
        PORTJ = PORTJ & 0xFE;       // 1111 1110
        PORTJ = PORTJ & 0xFD;       // 1111 1101
        PORTG = PORTG & 0xDF;       // 1101 1111
        PORTH = PORTH & 0xBF;       // 1011 1111
        PORTE = PORTE & 0xEF;       // 1110 1111
        PORTD = PORTD & 0xFD;       // 1111 1101
        PORTD = PORTD & 0xFE;       // 1111 1110
        PORTG = PORTG & 0xFB;       // 1111 1011
        PORTJ = PORTJ & 0x7F;       // 0111 1111

        sei();
     #endif

     debug_message_timestamp_PGM(str_gpio_init_end);
  }

/*-----------------------------------------------------------*/
/* function PLC_read_input()                                 */
/* Legge mediante primitive HW dipendent gli ingressi fisici */
/* e popola la struttura dell'immagine di processo del PLC   */
/*-----------------------------------------------------------*/
void PLC_read_input(PLC_IO* plc)
  {
    #ifdef GPIO_ARDUINO_MODE
        plc->X[1]  = digitalRead(GPIO_IN_1);
        plc->X[2]  = digitalRead(GPIO_IN_2);
        plc->X[3]  = digitalRead(GPIO_IN_3);
        plc->X[4]  = digitalRead(GPIO_IN_4);
        plc->X[5]  = digitalRead(GPIO_IN_5);
        plc->X[6]  = digitalRead(GPIO_IN_6);
        plc->X[7]  = digitalRead(GPIO_IN_7);
        plc->X[8]  = digitalRead(GPIO_IN_8);
        plc->X[9]  = digitalRead(GPIO_IN_9);
        plc->X[10] = digitalRead(GPIO_IN_10);
        plc->X[11] = digitalRead(GPIO_IN_11);
        plc->X[12] = digitalRead(GPIO_IN_12);
        plc->X[13] = digitalRead(GPIO_IN_13);
        plc->X[14] = digitalRead(GPIO_IN_14);
        plc->X[15] = digitalRead(GPIO_IN_15);
        plc->X[16] = digitalRead(GPIO_IN_16);
        plc->X[17] = digitalRead(GPIO_IN_17);
        plc->X[18] = digitalRead(GPIO_IN_18);
        plc->X[19] = digitalRead(GPIO_IN_19);
        plc->X[20] = digitalRead(GPIO_IN_20);
        plc->X[21] = digitalRead(GPIO_IN_21);
        plc->X[22] = digitalRead(GPIO_IN_22);
        plc->X[23] = digitalRead(GPIO_IN_23);
    #endif

    #ifdef GPIO_PORT_MODE
        cli();

        plc->X[1]  = PINF & 0x02;
        plc->X[2]  = PINF & 0x04;
        plc->X[3]  = PINF & 0x08;
        plc->X[4]  = PINF & 0x10;
        plc->X[5]  = PINF & 0x20;
        plc->X[6]  = PINF & 0x40;
        plc->X[7]  = PINF & 0x80;
        plc->X[8]  = PINK & 0x01;
        plc->X[9]  = PINK & 0x02;
        plc->X[10] = PINK & 0x04;
        plc->X[11] = PINK & 0x08;
        plc->X[12] = PINK & 0x10;
        plc->X[13] = PINK & 0x20;
        plc->X[14] = PINK & 0x40;
        plc->X[15] = PINK & 0x80;
        plc->X[16] = PINA & 0x01;
        plc->X[17] = PINA & 0x02;
        plc->X[18] = PINA & 0x04;
        plc->X[19] = PINA & 0x08;
        plc->X[20] = PINA & 0x10;
        plc->X[21] = PINA & 0x20;
        plc->X[22] = PINA & 0x40;
        plc->X[23] = PINA & 0x80;

        sei();
    #endif
  }

/*-------------------------------------------------------------*/
/* function PLC_write_output()                                 */
/* Legge dalla struttura dell'immagine di processo del PLC     */
/* e attiva fisicamente le uscite con primitive HW dipendente  */
/* nb : per compatibilita con la mappatura della scheda si e'  */
/*      deciso di non usare il vettore Y[0]                    */
/*-------------------------------------------------------------*/
void PLC_write_output(PLC_IO *plc)
{
    #ifdef GPIO_ARDUINO_MODE
        if (plc->Y[23])
            digitalWrite(GPIO_OUT_LED, HIGH);
        else
            digitalWrite(GPIO_OUT_LED, LOW);
    
        if (plc->Y[1])
            digitalWrite(GPIO_OUT_1, HIGH);
        else
            digitalWrite(GPIO_OUT_1, LOW);

        if (plc->Y[2])
            digitalWrite(GPIO_OUT_2, HIGH);
        else
            digitalWrite(GPIO_OUT_2, LOW);
    
        if (plc->Y[3])
            digitalWrite(GPIO_OUT_3, HIGH);
        else
            digitalWrite(GPIO_OUT_3, LOW);

        if (plc->Y[4])
            digitalWrite(GPIO_OUT_4, HIGH);
        else
            digitalWrite(GPIO_OUT_4, LOW);

        if (plc->Y[5])
            digitalWrite(GPIO_OUT_5, HIGH);
        else
            digitalWrite(GPIO_OUT_5, LOW);

        if (plc->Y[6])
            digitalWrite(GPIO_OUT_6, HIGH);
        else
            digitalWrite(GPIO_OUT_6, LOW);

        if (plc->Y[7])
            digitalWrite(GPIO_OUT_7, HIGH);
        else
            digitalWrite(GPIO_OUT_7, LOW);

        if (plc->Y[8])
            digitalWrite(GPIO_OUT_8, HIGH);
        else
            digitalWrite(GPIO_OUT_8, LOW);

        if (plc->Y[9])
            digitalWrite(GPIO_OUT_9, HIGH);
        else
            digitalWrite(GPIO_OUT_9, LOW);

        if (plc->Y[10])
            digitalWrite(GPIO_OUT_10, HIGH);
        else
            digitalWrite(GPIO_OUT_10, LOW);

        if (plc->Y[11])
            digitalWrite(GPIO_OUT_11, HIGH);
        else
            digitalWrite(GPIO_OUT_11, LOW);

        if (plc->Y[12])
            digitalWrite(GPIO_OUT_12, HIGH);
        else
            digitalWrite(GPIO_OUT_12, LOW);

        if (plc->Y[13])
            digitalWrite(GPIO_OUT_13, HIGH);
        else
            digitalWrite(GPIO_OUT_13, LOW);

        if (plc->Y[14])
            digitalWrite(GPIO_OUT_14, HIGH);
        else
            digitalWrite(GPIO_OUT_14, LOW);

        if (plc->Y[15])
            digitalWrite(GPIO_OUT_15, HIGH);
        else
            digitalWrite(GPIO_OUT_15, LOW);

        if (plc->Y[16])
            digitalWrite(GPIO_OUT_16, HIGH);
        else
            digitalWrite(GPIO_OUT_16, LOW);

        if (plc->Y[17])
            digitalWrite(GPIO_OUT_17, HIGH);
        else
            digitalWrite(GPIO_OUT_17, LOW);

        if (plc->Y[18])
            digitalWrite(GPIO_OUT_18, HIGH);
        else
            digitalWrite(GPIO_OUT_18, LOW);

        if (plc->Y[19])
            digitalWrite(GPIO_OUT_19, HIGH);
        else
            digitalWrite(GPIO_OUT_19, LOW);

        if (plc->Y[20])
            digitalWrite(GPIO_OUT_20, HIGH);
        else
            digitalWrite(GPIO_OUT_20, LOW);

        if (plc->Y[21])
            digitalWrite(GPIO_OUT_21, HIGH);
        else
            digitalWrite(GPIO_OUT_21, LOW);

        if (plc->Y[22])
            digitalWrite(GPIO_OUT_22, HIGH);
        else
            digitalWrite(GPIO_OUT_22, LOW);
    #endif
    
    #ifdef GPIO_PORT_MODE
        cli();

        //OUT_LED, PB6
        if (plc->Y[23])
            PORTB = PORTB | 0x40;     // 0100 0000
        else
            PORTB = PORTB & 0xBF;     // 1011 1111

        // OUT_1, PH7
        if (plc->Y[1])
            PORTH = PORTH | 0x80;     // 1000 0000
        else
            PORTH = PORTH & 0x7F;     // 0111 1111

        // OUT_2, PG3
        if (plc->Y[2])
            PORTG = PORTG | 0x08;       // 0000 1000
        else
            PORTG = PORTG & 0xF7;       // 1111 0111

        // OUT_3, PG4
        if (plc->Y[3])
            PORTG = PORTG | 0x10;       // 0001 0000
        else
            PORTG = PORTG & 0xEF;       // 1110 1111

        // OUT_4, PG0
        if (plc->Y[4])
            PORTG = PORTG | 0x01;       // 0000 0001
        else
            PORTG = PORTG & 0xFE;       // 1111 1110

        // OUT_5, PG1
        if (plc->Y[5])
            PORTG = PORTG | 0x02;       // 0000 0010
        else
            PORTG = PORTG & 0xFD;       // 1111 1101

        // OUT_6, PC0
        if (plc->Y[6])
            PORTC = PORTC | 0x01;       // 0000 0001
        else
            PORTC = PORTC & 0xFE;       // 1111 1110

        // OUT_7, PC1
        if (plc->Y[7])
            PORTC = PORTC | 0x02;       // 0000 0010
        else
            PORTC = PORTC & 0xFD;       // 1111 1101

        // OUT_8, PC2
        if (plc->Y[8])
            PORTC = PORTC | 0x04;       // 0000 0100
        else
            PORTC = PORTC & 0xFB;       // 1111 1011

        // OUT_9, PC3
        if (plc->Y[9])
            PORTC = PORTC | 0x08;       // 0000 1000
        else
            PORTC = PORTC & 0xF7;       // 1111 0111

        // OUT_10, PC4
        if (plc->Y[10])
            PORTC = PORTC | 0x10;       // 0001 0000
        else
            PORTC = PORTC & 0xEF;       // 1110 1111

        // OUT_11, PC5
        if (plc->Y[11])
            PORTC = PORTC | 0x20;       // 0010 0000
        else
            PORTC = PORTC & 0xDF;       // 1101 1111

        // OUT_12, PC6
        if(plc->Y[12])
            PORTC = PORTC | 0x40;       // 0100 0000
        else
            PORTC = PORTC & 0xBF;       // 1011 1111

        // OUT_13, PC7
        if(plc->Y[13])
            PORTC = PORTC | 0x80;       // 1000 0000
        else
            PORTC = PORTC & 0x7F;       // 0111 1111

        // OUT_14, PJ0
        if (plc->Y[14])
            PORTJ = PORTJ | 0x01;       // 0000 0001
        else
            PORTJ = PORTJ & 0xFE;       // 1111 1110

        // OUT_15, PJ1
        if (plc->Y[15])
            PORTJ = PORTJ | 0x02;       // 0000 0010
        else
            PORTJ = PORTJ & 0xFD;       // 1111 1101

        // OUT_16, PG5
        if (plc->Y[16])
            PORTG = PORTG | 0x20;       // 0010 0000
        else
            PORTG = PORTG & 0xDF;       // 1101 1111

        // OUT_17, PH6
        if (plc->Y[17])
            PORTH = PORTH | 0x40;       // 0100 0000
        else
            PORTH = PORTH & 0xBF;       // 1011 1111

        // OUT_18, PE4
        if (plc->Y[18])
            PORTE = PORTE | 0x10;       // 0001 0000
        else
            PORTE = PORTE & 0xEF;       // 1110 1111

        // OUT_19, PD1
        if (plc->Y[19])
            PORTD = PORTD | 0x02;       // 0000 0010
        else
            PORTD = PORTD & 0xFD;       // 1111 1101

        // OUT_20, PD0
        if (plc->Y[20])
            PORTD = PORTD | 0x01;       // 0000 0001
        else
            PORTD = PORTD & 0xFE;       // 1111 1110

        // OUT_21, PG2
        if (plc->Y[21])
            PORTG = PORTG | 0x04;       // 0000 0100
        else
            PORTG = PORTG & 0xFB;       // 1111 1011

        // OUT_22, PJ7
        if (plc->Y[22])
            PORTJ = PORTJ | 0x80;       // 1000 0000
        else
            PORTJ = PORTJ & 0x7F;       // 0111 1111

        sei();
    #endif
}

/*-------------------------------------------------------------*/
/* function PLC_print_state()                                  */
/* Stampa sulla seriale lo stato di ingressi e uscite del PLC. */
/*-------------------------------------------------------------*/

static char tmpbuff[120];

void PLC_print_state(BOOL ena, PLC_IO *plc)
  {
     if (ena)
       {
         memset(tmpbuff, 0, sizeof(tmpbuff));
         strncpy(tmpbuff, (char*)"Stato ingressi: ",sizeof(tmpbuff));
         uint8_t pos;
         pos = strlen(tmpbuff);
         for (int j = 0; j < PLC_MAX_X; j++) {
             if (plc->X[j])
                 tmpbuff[pos] = '1';
             else
                 tmpbuff[pos] = '0';
             pos++;

             if ((((j + 1) % 8) == 0) && j>0)
               {
                 tmpbuff[pos] = ' ';
                 pos++;
               }
         }
         debug_print_timestamp(ena, tmpbuff);


         strncpy(tmpbuff, (char*)"Stato uscite:   ", sizeof(tmpbuff));
         pos = strlen(tmpbuff);
         for (int j =0; j < PLC_MAX_X; j++) {
             if (plc->Y[j])
                 tmpbuff[pos] = '1';
             else
                 tmpbuff[pos] = '0';
             pos++;

             if ((((j + 1) % 8) == 0) && j > 0)
             {
                 tmpbuff[pos] = ' ';
                 pos++;
             }

         }
         debug_print_timestamp(ena, tmpbuff);
     }
  }