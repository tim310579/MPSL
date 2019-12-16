#ifndef GPIO_INIT_H
#define GPIO_INIT_H
#endif

#include <stm32l476xx.h>

typedef enum {
	GPIO_Mode_IN = 0x00,  /*!< GPIO Pin as General Purpose Input */
	GPIO_Mode_OUT = 0x01, /*!< GPIO Pin as General Purpose Output */
	GPIO_Mode_AF = 0x02,  /*!< GPIO Pin as Alternate Function */
	GPIO_Mode_AN = 0x03,  /*!< GPIO Pin as Analog input/output */
} GPIO_Mode_t;

typedef enum {
	GPIO_OType_PP = 0x00, /*!< GPIO Output Type Push-Pull */
	GPIO_OType_OD = 0x01  /*!< GPIO Output Type Open-Drain */
} GPIO_OType_t;

typedef enum {
	GPIO_Speed_Low = 0x00,    /*!< GPIO Speed Low */
	GPIO_Speed_Medium = 0x01, /*!< GPIO Speed Medium */
	GPIO_Speed_High = 0x02,   /*!< GPIO Speed High */
	GPIO_Speed_VeryHigh = 0x03    /*!< GPIO Speed VeryHigh */
} GPIO_Speed_t;

typedef enum {
	GPIO_PuPd_NOPULL = 0x00, /*!< No pull resistor */
	GPIO_PuPd_UP = 0x01,     /*!< Pull up resistor enabled */
	GPIO_PuPd_DOWN = 0x02    /*!< Pull down resistor enabled */
} GPIO_PuPd_t;

void __GPIO_Init__(GPIO_TypeDef *GPIOx, int pin, GPIO_Mode_t GPIO_Mode, GPIO_OType_t GPIO_OType, GPIO_Speed_t GPIO_Speed, GPIO_PuPd_t GPIO_PuPd){

	GPIOx->MODER &= ~(3 << (2 * pin));//clear
	GPIOx->MODER |= (GPIO_Mode << (2 * pin));//set

	GPIOx->OTYPER &= ~(1 << pin);//clear
	GPIOx->OTYPER |= (GPIO_OType << pin);//set

	GPIOx->OSPEEDR &= ~(3 << (2 * pin));//clear
	GPIOx->OSPEEDR |= (GPIO_Speed << (2 * pin));//set

	GPIOx->PUPDR &= ~(3 << (2 * pin));//clear
	GPIOx->PUPDR |= (GPIO_PuPd << (2 * pin));//set

}
