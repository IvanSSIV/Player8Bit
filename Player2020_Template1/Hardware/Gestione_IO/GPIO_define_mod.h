/****************************************************************************/
/* File: GPIO_define.h                                                      */
/* Data creazione: 24/03/2020                								*/
/* Creato da: Olivetti - De Stefani      									*/
/* Funzione: Definizione mnemonica della piedinatura della scheda Lanzi  	*/
/*			 con architettura a 8 bit.										*/
/* Changelog:																*/
/*																			*/
/*																			*/
/****************************************************************************/

// Definizione del metodo utilizzato per manipolare gli I/O
//#define GPIO_ARDUINO_MODE
#define GPIO_PORT_MODE


#ifdef GPIO_ARDUINO_MODE
	// Definizione degli ingressi
	#define GPIO_IN_1			55
	#define GPIO_IN_2			56
	#define GPIO_IN_3			57
	#define GPIO_IN_4			58
	#define GPIO_IN_5			59
	#define GPIO_IN_6			60
	#define GPIO_IN_7			61
	#define GPIO_IN_8			62
	#define GPIO_IN_9			63
	#define GPIO_IN_10			64
	#define GPIO_IN_11			65
	#define GPIO_IN_12			66
	#define GPIO_IN_13			67
	#define GPIO_IN_14			68
	#define GPIO_IN_15			69
	#define GPIO_IN_16			22
	#define GPIO_IN_17			23
	#define GPIO_IN_18			24
	#define GPIO_IN_19			25
	#define GPIO_IN_20			26
	#define GPIO_IN_21			27
	#define GPIO_IN_22			28
	#define GPIO_IN_23			29
	#define GPIO_IN_FORCELLA	54
	#define GPIO_IN_CURRENT		 0

	// Definizione delle uscite
	#define GPIO_OUT_1			 3 // Y1
	#define GPIO_OUT_2			 5
	#define GPIO_OUT_3			38
	#define GPIO_OUT_4			41
	#define GPIO_OUT_5			40
	#define GPIO_OUT_6			37
	#define GPIO_OUT_7			36
	#define GPIO_OUT_8			35
	#define GPIO_OUT_9			34
	#define GPIO_OUT_10			33
	#define GPIO_OUT_11			32
	#define GPIO_OUT_12			31
	#define GPIO_OUT_13			30
	#define GPIO_OUT_14			15
	#define GPIO_OUT_15			14
	#define GPIO_OUT_16			 4
	#define GPIO_OUT_17			 9
	#define GPIO_OUT_18			 2
	#define GPIO_OUT_19			49
	#define GPIO_OUT_20			48
	#define GPIO_OUT_21			39
	#define GPIO_OUT_22			47  // Y22
	#define GPIO_OUT_LED		12  // Y23
	#define GPIO_OUT_MOTOR		13  // Y24
#endif