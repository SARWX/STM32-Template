#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi_functions.h"
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
    
    spiInit(SPI2);
        uint8_t status = eepromReadStatus();
        uint8_t buf[4] = {0,0,0,7};
        uint8_t cnt = 15;      // 15 байт, максимумм можно 16 за раз

        eepromWriteStatus(0x0);
        status = eepromReadStatus();
        eepromUARTinit();
        eepromWriteWithUART(0x277);



        cnt = 0;
        int exit = 0;

        // eepromUARTinit();
        // int a = getchar();
        // int b = getchar();
        // int c = getchar();

        // for (int cur = 255; cur > 0; cur--) {
        //     a = buffer[cnt++] = getchar();
        //     if (a == EOF) {
        //         exit = 1;
        //         break;
        //     }
        // }
        // while(1);




// //      char *str = malloc(sizeof(char) * strlen(HELLO_STRING));
//         char str[16] = "Timofei Novikov";

//         eepromWriteEnable ();
//         status = eepromReadStatus();
        
//         eepromWrite (str, cnt, 0x10);

//         eepromWriteDisable ();
//         status = eepromReadStatus();

//         uint8_t r_buf[15];
//         eepromRead(r_buf, cnt, 0x10);

//         for (int i; i < 3000; i++)
//             ;

// // Andres Rojas
//         char andres[12] = "Andres Rojas";
//         eepromWriteEnable ();
//         eepromWrite (andres, cnt, 0x200);

//         eepromWriteDisable ();
//         status = eepromReadStatus();

//         eepromRead(r_buf, 12, 0x200);

//         eepromUARTinit();
        
    
}
