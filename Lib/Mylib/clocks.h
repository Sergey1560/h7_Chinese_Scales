#ifndef CLOCKS_H
#define CLOCKS_H

#include "common_defs.h"

#define STM32H743

#define CLOCKS_480 0
#define CLOCKS_200 1

#if CLOCKS_480
#include "clocks_480.h"
#endif

#if CLOCKS_200
#include "clocks_200.h"
#endif


/* Тактирование таймеров */
#define APB1_TIMER_CLK APB1_CLK
#define APB2_TIMER_CLK APB2_CLK


#define TIM2_CLK APB1_TIMER_CLK
#define TIM3_CLK APB1_TIMER_CLK
#define TIM4_CLK APB1_TIMER_CLK
#define TIM5_CLK APB1_TIMER_CLK
#define TIM6_CLK APB1_TIMER_CLK
#define TIM7_CLK APB1_TIMER_CLK
#define TIM12_CLK APB1_TIMER_CLK
#define TIM13_CLK APB1_TIMER_CLK
#define TIM14_CLK APB1_TIMER_CLK

#define TIM1_CLK APB2_TIMER_CLK
#define TIM8_CLK APB2_TIMER_CLK
#define TIM15_CLK APB2_TIMER_CLK
#define TIM16_CLK APB2_TIMER_CLK
#define TIM17_CLK APB2_TIMER_CLK


#endif