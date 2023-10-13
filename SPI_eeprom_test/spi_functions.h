#ifndef SPI_FUNC_H
#define SPI_FUNC_H

void spiInit(SPI_TypeDef *SPIx);
int spiReadWrite(SPI_TypeDef* SPIx , uint8_t *rbuf , const uint8_t *tbuf , uint8_t cnt , uint16_t speed);
void spiCS(int x);

#endif