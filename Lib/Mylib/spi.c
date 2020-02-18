#include "spi.h"


volatile float size_in_mm[2];
volatile uint8_t size_in_mm_index;

void SPI1_IRQHandler(void){
    uint32_t data;

    data = SPI1->RXDR;

    if(size_in_mm_index == 0){
        size_in_mm[0] = ((float)data / 20480.0) * 25.4;
    };
    if(size_in_mm_index == 1){
        size_in_mm[1] = ((float)data / 20480.0) * 25.4;
    };

    size_in_mm_index++;
    
    if(size_in_mm_index > 1){
        size_in_mm_index=0;
    }
    INFO("SPI get %d",data);
}


void spi1_init(void){
    spi_config_t *spi_conf=0;

    spi1_gpio_init();
    
    size_in_mm_index=0;
    
    // CLK source
    RCC->D2CCIP1R |= (SPI_CLK_SRC << RCC_D2CCIP1R_SPI123SEL_Pos);
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    spi_conf->SPI = SPI1;
    spi_conf->CR1 = SPI_CR1_SSI;
    spi_conf->CFG1 = (23 << SPI_CFG1_DSIZE_Pos)|(7 << SPI_CFG1_CRCSIZE_Pos);
    spi_conf->CFG2 = SPI_CFG2_SSM|\
                     SPI_CFG2_CPHA|\
                     SPI_CFG2_CPOL|\
                     SPI_CFG2_LSBFRST|\
                     (2 << SPI_CFG2_COMM_Pos);
    spi_conf->IER = SPI_IER_RXPIE;

    spi_setup(spi_conf);
    NVIC_EnableIRQ(SPI1_IRQn);
    NVIC_SetPriority(SPI1_IRQn,0);
}


void spi_setup(spi_config_t * spi_config){
    SPI_TypeDef* SPI = spi_config->SPI;

    if(SPI == SPI1){
        RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
        __NOP();
        RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
    }

    if(SPI == SPI2){
        RCC->APB1LRSTR |= RCC_APB1LRSTR_SPI2RST;
        __NOP();
        RCC->APB1LRSTR &= ~RCC_APB1LRSTR_SPI2RST;
    }

    SPI->CR1 = spi_config->CR1;
    SPI->CFG1 = spi_config->CFG1;
    SPI->CFG2 = spi_config->CFG2;
    SPI->IER = spi_config->IER;
    SPI->CR1 = (spi_config->CR1 | SPI_CR1_SPE);
}


void spi1_gpio_init(void){
    /* SPI1
    PA5 - SCK
    PA7 - MOSI
    */
   	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN; 
    //MODE - 10 AF
	GPIOA->MODER &= ~(GPIO_MODER_MODER5|GPIO_MODER_MODER7);
    GPIOA->MODER |= (GPIO_MODER_MODER5_1|GPIO_MODER_MODER7_1);
	//Push-pull mode 0
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_5|GPIO_OTYPER_OT_7);
	//Very High speed 11
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR5|GPIO_OSPEEDER_OSPEEDR7);
    GPIOA->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR5_Pos)|\
					  (S_VH << GPIO_OSPEEDER_OSPEEDR7_Pos);
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5|GPIO_AFRL_AFRL7);
    //AF5
	GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFRL5_Pos)|\
					 (5 << GPIO_AFRL_AFRL7_Pos);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5|GPIO_PUPDR_PUPDR7);
}


