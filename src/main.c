#include "main.h"

volatile uint8_t led_status;

int main(void){
	RCC_init();
	dwt_init(); 
	timer2_init(); //Инит таймера без запуска

	//Нога PA6 используется только для отладки
	//В момент включени SPI опускается, в момент
	//отключени поднимается.
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODER6);
    GPIOA->MODER |= (GPIO_MODER_MODER6_0);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_6);
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6);
    GPIOA->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR6_Pos);
	//Pull UP
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_0;

	init_exti();
	
	while(1){
	};
}


void init_exti(void){
	//PA5 используется как CLK, для определения 
	//начала пакета, сначала конфигурируется как вход
	//с прерыванием по восходящему уровню
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;	
	GPIOA->MODER &= ~(GPIO_MODER_MODER5);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_5);
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR5);
    GPIOA->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR5_Pos);
	//Pull UP
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_0;

    RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;  
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA; 
	EXTI->RTSR1 	|= EXTI_RTSR1_TR5;
	NVIC_EnableIRQ (EXTI9_5_IRQn);
	//Разрешаем прерывания от PA5 в периферии
	EXTI_D1->IMR1 |= EXTI_IMR1_IM5;
}

void timer2_init(void){
	RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
	TIM2->PSC = (uint16_t)(TIM2_CLK/1000000)-1;
	TIM2->ARR = 550 - 1; 	
	TIM2->DIER |= TIM_DIER_UIE;
	__DSB();
	
	
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority (TIM2_IRQn, 0);
}


void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_UIF)	TIM2->SR &= ~TIM_SR_UIF; 

	GPIOA->BSRR = GPIO_BSRR_BS_6;
	TIM2->CR1 &= ~TIM_CR1_CEN;  //Выключить таймер
	spi1_init();  //Сброс настроек spi, CS поднят
	EXTI_D1->IMR1 |= EXTI_IMR1_IM5; //Включить прерывание
}




void EXTI9_5_IRQHandler(void){
	static uint32_t prev_time=0;
	uint32_t delta;

	if(EXTI_D1->PR1 & EXTI_PR1_PR5){
			EXTI_D1->PR1 = EXTI_PR1_PR5;
	};

	if(prev_time > 0){
		delta = dwt_get_diff_sec(prev_time);
	}else{
		delta = 0;
	}
	
	prev_time = dwt_get_tick();

	if(delta > 100){
		EXTI_D1->IMR1 &= ~EXTI_IMR1_IM5; //Отключить прерывание EXTI
		spi1_init(); //Инит SPI, CS поднят.
		SPI1->CR1 &= ~SPI_CR1_SSI; //Опустить CS
		GPIOA->BSRR = GPIO_BSRR_BR_6; //Опустить ногу отладки
		TIM2->CR1 |= TIM_CR1_CEN; //Запустить таймер работы SPI
	}

	
}
