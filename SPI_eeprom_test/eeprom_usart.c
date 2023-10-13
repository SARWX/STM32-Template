#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

int getchar(void)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        ;
    return (USART1->DR & 0xff);
}


int eepromUARTinit(void) {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    // Initialize USART1_Tx
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    // Initialize USART1_RX
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // see stm32f10x_usart.h
    USART_InitTypeDef USART_InitStructure;
    // Initialize USART structure
    USART_StructInit(&USART_InitStructure);
    // Modify USART_InitStructure for non-default values , e.g.
    // USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}





    // while (1)
    // {
    //     int character = 0;
    //     character = getchar();
    //     putchar(character);
    // }
