/*--------------------------------------------------------------------------*/
/* Progetto   : Librerie di programmi in "C"                                */
/* Oggetto    : Header file con definizioni generalizzate                   */
/* Nome file  : mydef.h                                                     */
/* Operatore  : Adriano                                                     */
/* Data       : 25-03-2014                                                  */
/* Note       :                                                             */
/*--------------------------------------------------------------------------*/
// definizione tipo di "endian-less"
#define BYTE_LITTLE_ENDIAN

/* Types definition */
#ifndef _BASICTYPES_H_
typedef unsigned char  BYTE;    // unsigned 8 bit
typedef unsigned char  BOOL;    // boolean
typedef unsigned short WORD;    // unsigned 16 bit
typedef unsigned long  DWORD;   // unsigned 32 bit
typedef unsigned char  UINT8;	// unsigned 8-bit
typedef unsigned int   UINT16;  // unsigned 16-bit
typedef unsigned long  UINT32;  // unsigned 32-bit
typedef signed char    INT8;	// signed 8-bit
typedef int	       INT16;	// signed 16-bit
typedef long int       INT32;	// signed 32-bit
#endif

typedef signed char    SBYTE;   // signed 8 bit
typedef signed short   SWORD;   // signed 16 bit
typedef long           SDWORD;  // signed 32 bit
typedef float          SINGLE;  // floating point singola precisione
typedef double         DOUBLE;  // floating point doppia precisione

#ifndef STRUCT_PACK
#define STRUCT_PACK 
#endif



/* define per constanti simboliche generalizzate */
#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

#define ON       0xFF
#define OFF      0x00

#ifndef NULL
#define NULL     0
#endif

/* define per mascheratura dei bit di un byte */
#define BY_B0     0x01
#define BY_B1     0x02
#define BY_B2     0x04
#define BY_B3     0x08
#define BY_B4     0x10
#define BY_B5     0x20
#define BY_B6     0x40
#define BY_B7     0x80

#define BY_NB0    0xFE
#define BY_NB1    0xFD
#define BY_NB2    0xFB
#define BY_NB3    0xF7
#define BY_NB4    0xEF
#define BY_NB5    0xDF
#define BY_NB6    0xBF
#define BY_NB7    0x7F


/* define per i caratteri ASCII di controllo */
#define CH_NUL   0x00
#define CH_LF    0x0A
#define CH_FF    0x0C
#define CH_CR    0x0D
#define CH_ESC   0x1B

/*---------------------------------------------------*/
/* union per le equivalenze tra byte,word            */
/* viene utilizzata per migliorare l'efficenza di    */
/* accesso ai singoli byte di una word               */
/*---------------------------------------------------*/
union by_wo
{
    WORD  wo;
    BYTE  by[2];
};
typedef union by_wo BY_WO;

/*---------------------------------------------------*/
/* union per le equivalenze tra byte,word,e dword    */
/* viene utilizzata per migliorare l'efficenza di    */
/* accesso ai singoli byte o word di una doppia word */
/* nel caso di utilizzo di microcontrollore a 8 bit  */
/*---------------------------------------------------*/
union by_wo_dw
{
    DWORD dw;
    WORD  wo[2];
    BYTE  by[4];
};
typedef union by_wo_dw BY_WO_DW;

// definizione costanti per accesso ai byte o alle word di una longword
// per gestione diversa organizzazione tra processori Motorola e Intel
// il sistema "Little endian" e' usato da :
//    - processori Intel e derivati
//    - VAX
//    - ARM recenti (Corte-M3)
// il sistema "Big endian" e' usato da :
//    - processori Motorola e derivati
//    - SPARC
//    - ARM di prima generazione
//    - memorizzazione nativa dati TCP/IP
#ifdef BYTE_LITTLE_ENDIAN
#define DW_B0 3
#define DW_B1 2
#define DW_B2 1
#define DW_B3 0
#define DW_W0 1
#define DW_W1 0
#endif

#ifdef BYTE_BIG_ENDIAN
#define DW_B0 0
#define DW_B1 1
#define DW_B2 2
#define DW_B3 3
#define DW_W0 0
#define DW_W1 1
#endif

// definizione costanti per accesso ai byte di una word
// per gestione diversa organizzazione tra processori Motorola e Intel
#ifdef BYTE_LITTLE_ENDIAN
#define WO_B0 1
#define WO_B1 0
#endif

#ifdef BYTE_BIG_ENDIAN
#define WO_B0 0
#define WO_B1 1
#endif



