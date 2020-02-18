#ifndef SPI_H
#define SPI_H
#include "common_defs.h"
#include "clocks.h"

//Для доступа по 8бит
#define SPI1_TXDR_8b          (*(__IO uint8_t *)((uint32_t)SPI1+0x020))
#define SPI2_TXDR_8b          (*(__IO uint8_t *)((uint32_t)SPI2+0x020))
//Для доступа по 16бит
#define SPI1_TXDR_16b         (*(__IO uint16_t *)((uint32_t)SPI1+0x020))
#define SPI2_TXDR_16b         (*(__IO uint16_t *)((uint32_t)SPI2+0x020))

#define SPI1_RXDR_8b          (*(__IO uint8_t *)((uint32_t)SPI1+0x030))
#define SPI2_RXDR_8b          (*(__IO uint8_t *)((uint32_t)SPI2+0x030))
//Для доступа по 16бит
#define SPI1_RXDR_16b         (*(__IO uint16_t *)((uint32_t)SPI1+0x030))
#define SPI2_RXDR_16b         (*(__IO uint16_t *)((uint32_t)SPI2+0x030))


typedef struct spi_config_t
{
    SPI_TypeDef* SPI;
    uint32_t CR1;
    uint32_t CFG1;
    uint32_t CFG2;
    uint32_t IER;

}spi_config_t;


void spi1_init(void);
void spi1_gpio_init(void);
void spi_setup(spi_config_t * spi_config);

#endif