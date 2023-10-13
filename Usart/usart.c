#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

int putchar(int c) {
    while (USART_GetFlagStatus(USART1 , USART_FLAG_TXE) == RESET)       // USART_GetFlagStatus - функция, которая возвращает состояние (значение) одного из флагов
                                                                        // Это значение может быть значение SET и RESET
                                                                        // Собственно USART_FLAG_TXE - это флаг (Transmit data register empty flag, т.е RESET значит, что он не empty) 
                                                                        // FlagStatus - это тип данных, который может быть либо SET = 1 или RESET = 0 (объявлен в stm32f10x.h)
        ;
    USART1 ->DR = (c & 0xff);       // Сама структура USART_TypeDef определена в stm32f10x.h Там же определены указатели на USART1 и т.д.   
    return 0;
}


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

int Wait_for(uint32_t nTime){
    TimingDelay = nTime;
    int a = 0;
    while(TimingDelay != 0) {
         a |= (GPIOA->IDR & 0x01);
    }
    return(a);
}

#ifdef USE_FULL_ASSERT
    void assert_failed(uint8_t* file, uint32_t line)
    {
        /* Infinite loop */
        /* Use GDB to find out why we're here */
        while (1);
    }
#endif   


int main(void) {

    // + button
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC , ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    // Initialize USART1_Tx
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA , &GPIO_InitStruct);
    // Initialize USART1_RX
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStruct);

    // Конфигурируем кнопку
    GPIOA->CRL=0x04;            // GPIOA - это указатель на структуру, т.е. он указывает на место в памяти, в котором хранится структура GPIOA со всеми своими параметрами
    // Конфигурируем светодиоды
    GPIOC->CRH=0x22;            // Аналогично CRH - параметр и ничего особенно большего

    // Configure SysTick Timer
    if (SysTick_Config(SystemCoreClock / 1000))     // SysTick_Config настраивает таймер на генерацию исключений каждые SystemCoreClock / 1000 тактов
    while (1); 

    static int ledval = 0;

    // see stm32f10x_usart.h
    USART_InitTypeDef USART_InitStructure;
    // Initialize USART structure
    USART_StructInit(&USART_InitStructure);
    // Modify USART_InitStructure for non-default values , e.g.
    // USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1 ,&USART_InitStructure);
    USART_Cmd(USART1 , ENABLE);


    while (1) {
            /*
        for (int i = 0; i < 512; i++) {
            putchar(i);
        }
        for (int i = 0; i < 1024; i++) {
            putchar(0);
        }   */
        int word = 0;
       // putchar(32);
        for (int cur_bit = 1; cur_bit < 16; cur_bit <<= 1 ) {
            if (cur_bit == 1) {
                while (!(GPIOA->IDR & 0x01))
                    ;
                int x = 0;
                GPIOC->BSRR = 0x100;
                Delay(400);
                if (GPIOA->IDR & 0x01) {
                    x = 1;
                    GPIOC->BSRR |= 0x200;
                    Delay(300);
                }
                GPIOC->BSRR = (0x1000000 | 0x2000000);
                word |= (cur_bit * x);
            }
            else {
                if (Wait_for(500)) {
                    int x = 0;
                    GPIOC->BSRR = 0x100;
                    Delay(400);
                    if (GPIOA->IDR & 0x01) {
                        x = 1;
                        GPIOC->BSRR |= 0x200;
                        Delay(300);
                    }
                    GPIOC->BSRR = (0x1000000 | 0x2000000);
                    word |= (cur_bit * x);
                }
                else 
                    break;
            }
        }
        GPIOC->BSRR = 0x100;
        Delay(200);
        GPIOC->BSRR = 0x1000000;
        Delay(200);
        GPIOC->BSRR = 0x100;
        Delay(200);
        GPIOC->BSRR = 0x1000000;
        putchar(word);
    }
}