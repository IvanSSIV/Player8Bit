#include "Arduino.h"
#include "AVR_API.h"
#include "project_def.h"

// Versione platform-independent della strncpy 
char* strncpy_AVR_PGM(char* _dest, const char* _src, size_t _size)
{
	#ifdef ENV_AVR
	  return strncpy_P(_dest, _src, _size);
	#else
	  return strncpy(_dest, _src, _size);
	#endif
}

// Funzione di conversione da stringa di program memory a stringa "normale" del C
char* AVR_PGM_to_str(const char* string)
  {
     static char buff[128];
     #ifdef ENV_AVR
     strncpy_P(buff, string, sizeof(buff) - 1);
     #else
     strncpy(buff, string, sizeof(buff) - 1);
     #endif
     return buff;
  }

// Funzione di calcolo lunghezza stringa in PROGMEM
size_t AVR_PGM_strlen(const char* ptr)
{
	#ifdef ENV_AVR
		return strlen_P(ptr);
	#else
		return strlen(ptr);
	#endif
}

// Funzione di copia di stringa da PROGMEM
char* AVR_PGM_strncpy(char* dest, const char* src, size_t size)
{
	#ifdef ENV_AVR
		return strncpy_P(dest, src, size);
	#else
		return strncpy(dest, src, size);
	#endif
}