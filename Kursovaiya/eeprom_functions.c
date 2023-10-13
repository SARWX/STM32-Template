#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h> // +
#include <stm32f10x_spi.h>
#include "spi_functions.h"
#include "eeprom_functions.h"
#include "eeprom_usart.h"


uint8_t eepromReadStatus () { //
    uint8_t cmd[] = {cmdRDSR , 0xff};
    uint8_t res [2];                                                //  Заменить эти EEPROM_PORT и т.п. на переменные
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);
    spiReadWrite(EEPROM_SPI, res, cmd, 2, EEPROM_SPEED);
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
    return res [1];
}

uint8_t eepromWriteStatus (uint8_t status) {        // Скользкая тема с этим Write status register, надо почитать 
    uint8_t res [2];
    uint8_t cmd[] = {cmdWRSR , status};                                              //  Заменить эти EEPROM_PORT и т.п. на переменные
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);
    spiReadWrite(EEPROM_SPI, 0, cmd, 2, EEPROM_SPEED);
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
    return res [1];
}

#define WIP(x) ((x) & 1)
void eepromWriteEnable () { //
    uint8_t cmd = cmdWREN;
    while (WIP(eepromReadStatus ()));
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);
    spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
}

void eepromWriteDisable () { //
    uint8_t cmd = cmdWRDI;
    while (WIP(eepromReadStatus ()));
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);
    spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
}


int eepromRead(uint8_t *buf, uint8_t cnt, uint16_t offset) {
    uint8_t adress_1 = offset, adress_2 = offset >> 8;          // Separate 16 bit to the 2 eight bit symbols
    uint8_t cmd[3] = {cmdREAD, adress_2, adress_1};

    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);
    spiReadWrite(EEPROM_SPI, 0, cmd, 3, EEPROM_SPEED);
    spiReadWrite(EEPROM_SPI, buf, 0, cnt, EEPROM_SPEED);
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
    return(0);
}

int eepromWrite(uint8_t *buf, uint8_t cnt, uint16_t offset) {
    uint8_t adress_1 = offset, adress_2 = offset >> 8;          // Separate 16 bit to the 2 eight bit symbols
    uint8_t cmd[3] = {cmdWRITE, adress_2, adress_1};

    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);
    spiReadWrite(EEPROM_SPI, 0, cmd, 3, EEPROM_SPEED);
    spiReadWrite(EEPROM_SPI, 0, buf, cnt, EEPROM_SPEED);
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
    return(0);
}

// Data in eeprom is stored in 16 bytes pages
// And to perform write operation the last written byte must be
// not far from x.11111111 adress, where x - is the number of page
// the following code perform step to the next page automatically

int eepromWriteWithUART (uint16_t adress) {
    uint8_t adress_1 = adress, adress_2 = adress >> 8;          // Separate 16 bit to the 2 eight bit symbols
    uint8_t cur = (adress_1 & 0xf);
    uint8_t cnt = 0;
    uint8_t buffer[16];
    int exit = 0;

    while (exit != 1) {
        int simbol;
        //  get array from uart
            for (cur; cur < 15; cur++) {
                simbol = getchar();
                buffer[cnt++] = simbol;
                if (simbol == EOF) {
                    exit = 1;
                    break;
                }
            }
            
        //  write array to the eeprom module
            eepromWriteEnable ();
            eepromWrite (buffer, cnt, adress);
            eepromWriteDisable ();

        // next page
            adress = ((adress + 0x10) & (~0xf));  // ~oxf =  1111 1111  1111 0000
            cur = 0;
            cnt = 0;
    }
    return(0);
}