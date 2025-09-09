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
#define RCC_BASE 0x40023800U
#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC_BASE + 0x30U))

#define GPIO_BASE 0x40020000U
#define GPIOA ((GPIO_TypeDef*)GPIO_BASE)
#define GPIOB ((GPIO_TypeDef*)(GPIO_BASE + 0x400U))
#define GPIOC ((GPIO_TypeDef*)(GPIO_BASE + 0x800U))
#define GPIOD ((GPIO_TypeDef*)(GPIO_BASE + 0xC00U))
#define GPIOE ((GPIO_TypeDef*)(GPIO_BASE + 0x1000U))
#define GPIOF ((GPIO_TypeDef*)(GPIO_BASE + 0x1400U))
#define GPIOG ((GPIO_TypeDef*)(GPIO_BASE + 0x1800U))
#define GPIOH ((GPIO_TypeDef*)(GPIO_BASE + 0x1C00U))

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

typedef enum {
	GPIO_MODE_INPUT = 0x00U,
	GPIO_MODE_OUTPUT = 0x01U,
	GPIO_MODE_AF = 0x02U,
	GPIO_MODE_ANALOG = 0x03U
} GPIO_Mode;

typedef enum {
	GPIO_OTYPE_PP = 0x00U, // Push pull
	GPIO_OTYPE_OD = 0x01U // Open Drain
} GPIO_OType;

typedef enum {
	GPIO_OSPEED_LOW = 0x00U,
	GPIO_OSPEED_MED = 0x01U,
	GPIO_OSPEED_FAST = 0x02U,
	GPIO_OSPEED_HIGH = 0x03U
} GPIO_OSpeed;

typedef enum {
	GPIO_PUPD_NONE = 0x00U,
	GPIO_PUPD_PU = 0x01U, // Pull up
	GPIO_PUPD_PD = 0x02U // Pull down
} GPIO_Pupd;

typedef enum {
	GPIO_PIN_0 = 0U,
	GPIO_PIN_1 = 1U,
	GPIO_PIN_2 = 2U,
	GPIO_PIN_3 = 3U,
	GPIO_PIN_4 = 4U,
	GPIO_PIN_5 = 5U,
	GPIO_PIN_6 = 6U,
	GPIO_PIN_7 = 7U,
	GPIO_PIN_8 = 8U,
	GPIO_PIN_9 = 9U,
	GPIO_PIN_10 = 10U,
	GPIO_PIN_11 = 11U,
	GPIO_PIN_12 = 12U,
	GPIO_PIN_13 = 13U,
	GPIO_PIN_14 = 14U,
	GPIO_PIN_15 = 15U
} GPIO_Pin;



// HAL FUNCTIONS -----------------------------------------------------------
/**
 * @brief enables the AHB1 peripheral clock for given port
 * 
 * @param port - port to enable clock for
 * @return HAL_Status - HAL_OK or HAL_ERROR
 */
HAL_Status GPIO_enable_clock(GPIO_TypeDef* port);


/**
 * @brief Init function which configures given GPIO pin's settings
 * 		  Use the above macros ONLY for function input
 * 		  You MUST enable the AHB1 clock for the port you are trying to configure FIRST
 * 
 * @param port base address of GPIO port (pointing to GPIO_TypeDef struct)
 * @param pin 0-15 specifying which pin's settings to initialise
 * @param mode pin mode (out/in/AF/analog)
 * @param otype push pull/open drain output signalling
 * @param ospeed output clock speed
 * @param pupd pullup/pulldown on pin
 * @return HAL_Status - HAL_OK or HAL_ERROR
 */
HAL_Status GPIO_init(GPIO_TypeDef* port, GPIO_Pin pin, GPIO_Mode mode, GPIO_OType otype, GPIO_OSpeed ospeed, GPIO_Pupd pupd);


/**
 * @brief Writes high/low to given pin at given port - USE MACROS
 * 
 * @param port base address of GPIO port (pointing to GPIO_TypeDef struct)
 * @param pin 0-15 specifying which pin's settings to initialise
 * @param val PIN_SET or PIN_RESET
 * @return Hal_Status - HAL_OK or HAL_ERROR
 */
HAL_Status GPIO_write_pin(GPIO_TypeDef* port, GPIO_Pin pin, PIN_State val);


/**
 * @brief Writes entire port's 16 pins
 * 
 * @param port 
 * @param val 16 bit uint representing LSB = PIN0, MSB = PIN15 states
 * @return HAL_Status 
 */
HAL_Status GPIO_write_port(GPIO_TypeDef* port, uint16_t val);


/**
 * @brief Toggles given pin from high->low or low->high depending on current state
 * 
 * @param port 
 * @param pin
 * @return HAL_Status 
 */
HAL_Status GPIO_toggle_pin(GPIO_TypeDef* port, GPIO_Pin pin);


/**
 * @brief Reads value at pin, port and returns it
 * 
 * @param port 
 * @param pin 
 * @return PIN_State - PIN_SET (1) or PIN_RESET (0) or -1 if error!!
 */
PIN_State GPIO_read_pin(GPIO_TypeDef* port, GPIO_Pin pin);

#endif /* GPIO_DRIVER_GPIO_DRIVER_H_ */
