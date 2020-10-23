void EEPROM_Init(void);
void EEPROM_save_8bit(uint8_t* address, uint8_t value);
void EEPROM_save_16bit(uint16_t* address, uint16_t value);
void EEPROM_save_32bit(uint32_t* address, uint32_t value);
uint8_t EEPROM_read_8bit(uint8_t* address);
uint16_t EEPROM_read_16bit(uint16_t* address);
uint32_t EEPROM_read_32bit(uint32_t* address);