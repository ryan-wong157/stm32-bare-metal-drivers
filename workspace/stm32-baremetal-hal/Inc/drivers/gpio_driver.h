/*
 * gpio_driver.h
 *
 * Header file for gpio_driver.c
 * Contains function prototypes, register struct definitions, macros
 *
 *  Created on: Sep 6, 2025
 *      Author: Ryan Wong
 */

#ifndef GPIO_DRIVER_GPIO_DRIVER_H_
#define GPIO_DRIVER_GPIO_DRIVER_H_

// REGISTERS --------------------------------------------------------
#define RCC_BASE 0x40023800UL
#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC_BASE + 0x30))

#define GPIO_BASE 0x40020000UL
#define GPIOA ((GPIO_TypeDef*)GPIO_BASE)
#define GPIOB ((GPIO_TypeDef*)(GPIO_BASE + 0x400))
#define GPIOC ((GPIO_TypeDef*)(GPIO_BASE + 0x800))
#define GPIOD ((GPIO_TypeDef*)(GPIO_BASE + 0xC00))
#define GPIOE ((GPIO_TypeDef*)(GPIO_BASE + 0x1000))
#define GPIOF ((GPIO_TypeDef*)(GPIO_BASE + 0x1400))
#define GPIOG ((GPIO_TypeDef*)(GPIO_BASE + 0x1800))
#define GPIOH ((GPIO_TypeDef*)(GPIO_BASE + 0x1C00))


// TYPES -----------------------------------------------------------
typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
} GPIO_TypeDef;


// HAL FUNCTIONS -----------------------------------------------------------


#endif /* GPIO_DRIVER_GPIO_DRIVER_H_ */
