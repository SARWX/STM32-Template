#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi_functions.h"

void spiInit(SPI_TypeDef *SPIx) {

    // Create new variables
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;    
    // Initialize new variables
    GPIO_StructInit (& GPIO_InitStructure);
    SPI_StructInit (& SPI_InitStructure);   

    if (SPIx == SPI2) {
        // Enable clocks
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE); 

        // configure CLK
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // QESTION??????
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        // configure MISO
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        // configure MOSI
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        // configure SS
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

    } else {
        return;
    }
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPIx , &SPI_InitStructure);
    SPI_Cmd(SPIx , ENABLE);
}


int spiReadWrite(SPI_TypeDef* SPIx , uint8_t *rbuf , const uint8_t *tbuf , uint8_t cnt , uint16_t speed) {
    int i;
    SPIx ->CR1 = (SPIx ->CR1 & ~SPI_BaudRatePrescaler_256) | speed;
    for (i = 0; i < cnt; i++) {
        if (tbuf) {
            SPI_I2S_SendData(SPIx , *tbuf++);
        } else {
            SPI_I2S_SendData(SPIx , 0xff);
        }
        while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET)
            ;
        if (rbuf) {
            *rbuf++ = SPI_I2S_ReceiveData(SPIx);
        } else {
            SPI_I2S_ReceiveData(SPIx);
        }
    }
    return i;
}


void spiCS(int x) {
    if (x != 0)
        GPIO_WriteBit(GPIOB , GPIO_Pin_12 , 0);         // 1 = cs enable
    else
        GPIO_WriteBit(GPIOB , GPIO_Pin_12 , 1);         // 0 = cs disable
}
