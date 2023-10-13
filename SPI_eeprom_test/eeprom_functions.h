#ifndef EEPROM_FUNC_H
#define EEPROM_FUNC_H

#define EEPROM_PORT GPIOB
#define EEPROM_CS GPIO_Pin_12
#define EEPROM_SPI SPI2
#define EEPROM_SPEED SPI_BaudRatePrescaler_64  // super slow 64 = 1,333 micro sec)

#define START_DATA 0x0000
#define END_DATA 0x07ff

enum eepromCMD { 
cmdWRSR     = 0x01,     // Write status register    0x01
cmdWRITE    = 0x02,     // Data write               0x02
cmdREAD     = 0x03,     // Data read                0x03
cmdWRDI     = 0x04,     // Write disable            0x04
cmdRDSR     = 0x05,     // Read status register     0x05
cmdWREN     = 0x06      // Write enable             0x06
};
uint8_t eepromReadStatus ();
uint8_t eepromWriteStatus (uint8_t status);
void eepromWriteEnable ();
void eepromWriteDisable ();
int eepromRead(uint8_t *buf, uint8_t cnt, uint16_t offset);
int eepromWrite(uint8_t *buf, uint8_t cnt, uint16_t offset);

#endif