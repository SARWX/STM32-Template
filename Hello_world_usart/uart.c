#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include "uart.h"

int uart_open(USART_TypeDef* USARTx , uint32_t baud, uint32_t flags) {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // Initialize GPIO pins for our USARTx
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    int x = 0;
    if (USARTx == USART2)
        x = 2;
    else if (USARTx == USART3)
        x = 3;
    else 
        x = 1;

    switch ( x ) {
    case 2: {    
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);       
        // Tx
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;     
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA , &GPIO_InitStruct);
        // Rx
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA , &GPIO_InitStruct);   
    break; }
    case 3: {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        // Tx
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;     
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB , &GPIO_InitStruct);
        // Rx
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB , &GPIO_InitStruct); 
    break; }           
        
    default: {   // anything else means USART1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        // Tx
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;     
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA , &GPIO_InitStruct);
        // Rx
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA , &GPIO_InitStruct); 
    break; }
    }

    USART_InitTypeDef USART_InitStructure;      // structure for initializing
    USART_StructInit(&USART_InitStructure);
    // Baud rate
    USART_InitStructure.USART_BaudRate = baud;
    // Selecting Mode
    switch ( flags ) {
    case 1:             // 1 = Recive mode
        USART_InitStructure.USART_Mode = USART_Mode_Rx;
    break;
    case 2:             // 2 = Transmit mode
        USART_InitStructure.USART_Mode = USART_Mode_Tx;
    break;
    default:            // anything else means turn on both recive and transmit
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    break;
    }
    // Thats all, now initialize USARTx
    USART_Init(USARTx ,&USART_InitStructure);
    // Turn on our USARTx
    USART_Cmd(USARTx , ENABLE);
    // Turn on clock for USART (and also for gpioa wich is provide pins for our USART)
    return(0);
}


// int uart_close(USART_TypeDef* USARTx);


int uart_putc(int c, USART_TypeDef* USARTx) {
    while (USART_GetFlagStatus(USARTx , USART_FLAG_TXE) == RESET)   // NOOOO RESET    // USART_GetFlagStatus - функция, которая возвращает состояние (значение) одного из флагов
                                                                        // Это значение может быть значение SET и RESET
                                                                        // Собственно USART_FLAG_TXE - это флаг (Transmit data register empty flag, т.е RESET значит, что он не empty) 
                                                                        // FlagStatus - это тип данных, который может быть либо SET = 1 или RESET = 0 (объявлен в stm32f10x.h)
        ;
    USART1 ->DR = (c & 0xff);       // Сама структура USART_TypeDef определена в stm32f10x.h Там же определены указатели на USART1 и т.д.   
    return 0;
}

int uart_getc(USART_TypeDef* USARTx) {
    while (USART_GetFlagStatus(USARTx , USART_FLAG_RXNE) == RESET)
        ;
    return(USART1 -> DR & 0xff);
}
