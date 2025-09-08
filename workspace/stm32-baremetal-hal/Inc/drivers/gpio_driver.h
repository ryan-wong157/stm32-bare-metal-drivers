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

#include <stdint.h>

#include "drivers/types.h"

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

// MACROS ---------------------------------------------------------
#define GPIO_MODE_INPUT 0x00UL
#define GPIO_MODE_OUTPUT 0x01UL
#define GPIO_MODE_AF 0x02UL
#define GPIO_MODE_ANALOG 0x03UL
#define GPIO_OTYPE_PP 0x00UL // Push pull
#define GPIO_OTYPE_OD 0x01UL // Open Drain
#define GPIO_OSPEED_LOW 0x00UL
#define GPIO_OSPEED_MED 0x01UL
#define GPIO_OSPEED_FAST 0x02UL
#define GPIO_OSPEED_HIGH 0x03UL
#define GPIO_PUPD_NONE 0x00UL
#define GPIO_PUPD_PU 0x01UL // Pull up
#define GPIO_PUPD_PD 0x02UL // Pull down

#define GPIO_PIN_0 0UL
#define GPIO_PIN_1 1UL
#define GPIO_PIN_2 2UL
#define GPIO_PIN_3 3UL
#define GPIO_PIN_4 4UL
#define GPIO_PIN_5 5UL
#define GPIO_PIN_6 6UL
#define GPIO_PIN_7 7UL
#define GPIO_PIN_8 8UL
#define GPIO_PIN_9 9UL
#define GPIO_PIN_10 10UL
#define GPIO_PIN_11 11UL
#define GPIO_PIN_12 12UL
#define GPIO_PIN_13 13UL
#define GPIO_PIN_14 14UL
#define GPIO_PIN_15 15UL



// HAL FUNCTIONS -----------------------------------------------------------

/**
 * @brief Init function which enables the AHB1 peripheral clock and configures given GPIO pin's settings
 * 		  Use the above macros ONLY for function input
 * 
 * @param port base address of GPIO port (pointing to GPIO_TypeDef struct)
 * @param pin 0-15 specifying which pin's settings to initialise
 * @param mode pin mode (out/in/AF/analog)
 * @param otype push pull/open drain output signalling
 * @param ospeed output clock speed
 * @param pupd pullup/pulldown on pin
 * 
 * @return HAL_STATE_TypeDef - HAL_OK or HAL_ERROR
 */
HAL_Status GPIO_init(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd);

/**
 * @brief Writes high/low to given pin at given port - USE MACROS
 * 
 * @param port base address of GPIO port (pointing to GPIO_TypeDef struct)
 * @param pin 0-15 specifying which pin's settings to initialise
 * @param val PIN_SET or PIN_RESET
 */
void GPIO_write_pin(GPIO_TypeDef* port, uint32_t pin, PIN_State val);


#endif /* GPIO_DRIVER_GPIO_DRIVER_H_ */
