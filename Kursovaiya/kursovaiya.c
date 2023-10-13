#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>
#include "i2c_functions.h"
#include "eeprom_functions.h"
#include "eeprom_usart.h"

uint8_t txbuf [4], rxbuf [4];
uint16_t txbuf16 [4], rxbuf16 [4];
uint8_t buffer[256];


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    /* Infinite loop /
    / Use GDB to find out why we're here */
    while (1);
}
#endif

void main()
{
    
    spiInit(SPI2);  // III222CCC  Заменить на инициализацию I2C конкртного
        uint8_t status = eepromReadStatus();    
        uint8_t buf[4] = {0,0,0,7};
        uint8_t cnt = 15;      // 15 байт, максимумм можно 16 за раз

        eepromWriteStatus(0x0);
        status = eepromReadStatus();
        eepromUARTinit();
        eepromWriteWithUART(0x277);



        cnt = 0;
        int exit = 0;
}