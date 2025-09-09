/*
 * gpio_driver.c
 *
 * Contains HAL function which enables and uses the GPIO peripheral
 *
 *  Created on: Sep 6, 2025
 *      Author: Ryan Wong
 */

#include <stdlib.h>
#include "drivers/gpio_driver.h"

/**
 * This is a bit hacky, I'm not sure if it's ok or not
 * it basically assumes base addresses for all GPIO registers are 0x400 spaced (which they are for the stm32f4)
 * I think it's fine though because that was the assumption in gpio_driver.h when assigning the addresses anyway
 * 
 * It also has very weak input validation and mostly requires the user to use the macros properly
 */
HAL_Status GPIO_enable_clock(GPIO_TypeDef* port) {
    if ((uint32_t)port > (uint32_t)GPIOH || (uint32_t)port < (uint32_t)GPIOA) return HAL_ERROR;

    uint32_t shift = ((uint32_t)port - GPIO_BASE) / 0x400U;
    RCC_AHB1ENR |= 0x01U << shift;
    return HAL_OK;
}


/**
 * I have chosen to use enums for the options for each pin configuration. 
 * This is for readability and ease of use, although they are not strict compile-time type checks
 */
HAL_Status GPIO_init(GPIO_TypeDef* port, GPIO_Pin pin, GPIO_Mode mode, GPIO_OType otype, GPIO_OSpeed ospeed, GPIO_Pupd pupd) {
     if (
         port == NULL ||
         pin > GPIO_PIN_15 ||
         mode > GPIO_MODE_ANALOG ||
         otype > GPIO_OTYPE_OD ||
         ospeed > GPIO_OSPEED_HIGH ||
         pupd > GPIO_PUPD_PD
     ) return HAL_ERROR;

    // Clear existing bits, then configure MODER
    port->MODER &= ~(0x03U << (pin * 2));
    port->MODER |= ((uint32_t)mode << (pin * 2));

    // Configure OTYPE
    port->OTYPER &= ~(0x01U << pin);
    port->OTYPER |= ((uint32_t)otype << pin);

    // Configure OSPEED
    port->OSPEEDR &= ~(0x03U << (pin * 2));
    port->OSPEEDR |= ((uint32_t)ospeed << (pin * 2));

    // Configure PUPD
    port->PUPDR &= ~(0x03U << (pin * 2));
    port->PUPDR |= ((uint32_t)pupd << (pin * 2));

    return HAL_OK;
}

/**
 * Uses BSRR instead of ODR for atomic writes so it isn't interrupted by any ISR
 */
HAL_Status GPIO_write_pin(GPIO_TypeDef* port, GPIO_Pin pin, PIN_State val) {
    if (
         port == NULL ||
         pin > GPIO_PIN_15 ||
         val > PIN_SET
    ) return HAL_ERROR;

    // Using atomic set/reset for safety (note BSRR is WRITE-ONLY. You shouldn't set bits using |= as it reads)
    // BSRR is cleared in the next clock cycle (it's like a momentary write command)
    if (val == PIN_RESET) {
        port->BSRR = (1U << ((uint32_t)pin + 16U));
    } else {
        port->BSRR = (1U << (uint32_t)pin);
    }
    return HAL_OK;
}

/**
 * Again, this pedantically uses BSRR to prevent issues if interrupts touch the ODR
 */
HAL_Status GPIO_write_port(GPIO_TypeDef* port, uint16_t val) {
    if (
        port == NULL
    ) return HAL_ERROR;
    
    uint16_t inv = ~val;
    port->BSRR = (uint32_t)val | ((uint32_t)(inv) << 16);
    return HAL_OK;
}

HAL_Status GPIO_toggle_pin(GPIO_TypeDef* port, GPIO_Pin pin) {
    if (
        port == NULL ||
        pin > GPIO_PIN_15
    ) return HAL_ERROR;

    port->ODR ^= 0x01U << (uint32_t)pin;
    return HAL_OK;
}

/**
 * I made this return the pin value instead of using a pointer to a buffer and filling that out for simplicity
 * The tradeoff is I have to return -1 to indicate error instead of HAL_ERROR
 */
PIN_State GPIO_read_pin(GPIO_TypeDef* port, GPIO_Pin pin) {
    if (
        port == NULL ||
        pin > GPIO_PIN_15
    ) return -1;

    if (port->IDR & (0x01U << pin)) {
        return PIN_SET;
    } 
    return PIN_RESET;
}