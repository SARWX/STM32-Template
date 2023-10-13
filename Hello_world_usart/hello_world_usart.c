#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include "uart.h"

#ifdef USE_FULL_ASSERT
    void assert_failed(uint8_t* file, uint32_t line)
    {
        /* Infinite loop */
        /* Use GDB to find out why we're here */
        while (1);
    }
#endif   

// Timer code
static __IO uint32_t TimingDelay;
void Delay(uint32_t nTime){
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

void SysTick_Handler(void){
    if (TimingDelay != 0x00)
    TimingDelay --;
}

int main(void) {
    // Configure SysTick Timer
    if (SysTick_Config(SystemCoreClock / 1000))     // SysTick_Config настраивает таймер на генерацию исключений каждые SystemCoreClock / 1000 тактов
    while (1);      // если все ок и генерациянастроена, то SysTick_Config вернет 0, а если возникли ошибки, то вернет 1 и запустится бесконечный цикл 

    uart_open(USART1, 9600, 2);
    char hello[] = "Hello world!\n0";

    while (1) {
        for (int i = 0; hello[i] != '0'; i++) {
            while (USART_GetFlagStatus(USART1 , USART_FLAG_TXE) == RESET)
                ;
            USART1->DR = (hello[i] & 0xff); 
            Delay(300);
        }

    }
}