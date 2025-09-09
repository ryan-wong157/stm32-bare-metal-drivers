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


// REGISTERS ==============================================================
#define RCC_BASE 0x40023800U
#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC_BASE + 0x30U))

#define GPIO_BASE 0x40020000U
#define GPIOA ((GPIO_Reg_TypeDef*)GPIO_BASE)
#define GPIOB ((GPIO_Reg_TypeDef*)(GPIO_BASE + 0x400U))
#define GPIOC ((GPIO_Reg_TypeDef*)(GPIO_BASE + 0x800U))
#define GPIOD ((GPIO_Reg_TypeDef*)(GPIO_BASE + 0xC00U))
#define GPIOE ((GPIO_Reg_TypeDef*)(GPIO_BASE + 0x1000U))
#define GPIOF ((GPIO_Reg_TypeDef*)(GPIO_BASE + 0x1400U))
#define GPIOG ((GPIO_Reg_TypeDef*)(GPIO_BASE + 0x1800U))
#define GPIOH ((GPIO_Reg_TypeDef*)(GPIO_BASE + 0x1C00U))

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
} GPIO_Reg_TypeDef;


// GPIO Config Types ==============================================================
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

/**
 * Make sure to check the datasheet to see what each AF does for your selected pin 
 */
typedef enum {
	GPIO_AF0 = 0x00U,
	GPIO_AF1 = 0x01U,
	GPIO_AF2 = 0x02U,
	GPIO_AF3 = 0x03U,
	GPIO_AF4 = 0x04U,
	GPIO_AF5 = 0x05U,
	GPIO_AF6 = 0x06U,
	GPIO_AF7 = 0x07U,
	GPIO_AF8 = 0x08U,
	GPIO_AF9 = 0x09U,
	GPIO_AF10 = 0x0AU,
	GPIO_AF11 = 0x0BU,
	GPIO_AF12 = 0x0CU,
	GPIO_AF13 = 0x0DU,
	GPIO_AF14 = 0x0EU,
	GPIO_AF15 = 0x0FU,
} GPIO_AFx;

// Maybe in the future I will write this to support multi-pin init (using OR bitmasks)
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

/**
 * make sure to use the above enums when setting this init struct
 * 
 * mode - pin mode (out/in/AF/analog)
 * type - push pull/open drain output signalling
 * speed - output clock speed
 * pupd - pullup/pulldown on pin
 * afx - alternate function value
 */
typedef struct {
	GPIO_Mode mode;
	GPIO_OType otype;
	GPIO_OSpeed ospeed;
	GPIO_Pupd pupd;
	GPIO_AFx afx;
} GPIO_Init_TypeDef;


// HAL FUNCTIONS ==============================================================
/**
 * @brief enables the AHB1 peripheral clock for given port
 * 
 * @param port - port to enable clock for
 * @return HAL_Status - HAL_OK or HAL_ERROR
 */
HAL_Status GPIO_enable_clock(GPIO_Reg_TypeDef* port);


/**
 * @brief Init function which configures given GPIO pin's settings
 * 		  Use the above macros ONLY for function input
 * 		  You MUST enable the AHB1 clock for the port you are trying to configure FIRST
 * 
 * @param port base address of GPIO port (pointing to GPIO_Reg_TypeDef struct)
 * @param pin 0-15 specifying which pin's settings to initialise
 * @param init_struct pointer to init struct which contains config info for this GPIO pin
 * @return HAL_Status - HAL_OK or HAL_ERROR
 */
HAL_Status GPIO_init(GPIO_Reg_TypeDef* port, GPIO_Pin pin, const GPIO_Init_TypeDef* init_struct);


/**
 * @brief Writes high/low to given pin at given port - USE MACROS
 * 
 * @param port base address of GPIO port (pointing to GPIO_Reg_TypeDef struct)
 * @param pin 0-15 specifying which pin's settings to initialise
 * @param val PIN_SET or PIN_RESET
 * @return Hal_Status - HAL_OK or HAL_ERROR
 */
HAL_Status GPIO_write_pin(GPIO_Reg_TypeDef* port, GPIO_Pin pin, PIN_State val);


/**
 * @brief Writes entire port's 16 pins
 * 
 * @param port 
 * @param val 16 bit uint representing LSB = PIN0, MSB = PIN15 states
 * @return HAL_Status 
 */
HAL_Status GPIO_write_port(GPIO_Reg_TypeDef* port, uint16_t val);


/**
 * @brief Toggles given pin from high->low or low->high depending on current state
 * 
 * @param port 
 * @param pin
 * @return HAL_Status 
 */
HAL_Status GPIO_toggle_pin(GPIO_Reg_TypeDef* port, GPIO_Pin pin);


/**
 * @brief Reads value at pin, port and returns it
 * 
 * @param port 
 * @param pin 
 * @return PIN_State - PIN_SET (1) or PIN_RESET (0) or -1 if error!!
 */
PIN_State GPIO_read_pin(GPIO_Reg_TypeDef* port, GPIO_Pin pin);


/**
 * @brief Reads all 16 pins and writes into a 16 bit buffer pointer given. LSB = PIN0, MSB = PIN15
 * 
  * @param buffer - 16 bit pointer which is written into
  * @return HAL_Status 
 */
HAL_Status GPIO_read_port(GPIO_Reg_TypeDef* port, uint16_t* buffer);


/**
 * @brief Locks pins configuration registers (mode, otype, ospeed, pupd, afh, afl) based on the given bitmask
 * 
 * @param port - port to lock pins
 * @param pins - 16 bit bitmask which corresponds to which pins to lock configuration for. LSB = PIN0, MSB = PIN15
 * 				 0000 1000 0001 0001 would lock pin 0, 4, 11
 */
HAL_Status GPIO_lock_pins(GPIO_Reg_TypeDef* port, uint16_t pins);


#endif /* GPIO_DRIVER_GPIO_DRIVER_H_ */
