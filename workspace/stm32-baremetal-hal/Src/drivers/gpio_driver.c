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


HAL_Status GPIO_write_pin(GPIO_TypeDef* port, GPIO_Pin pin, PIN_State val) {
    if (
         port == NULL ||
         pin > GPIO_PIN_15 ||
         val > PIN_SET
    ) return HAL_ERROR;

    // Using atomic set/reset for safety (note BSRR is WRITE-ONLY)
    // BSRR is cleared in the next clock cycle (it's like a momentary write command)
    if (val == PIN_RESET) {
        port->BSRR = (1U << ((uint32_t)pin + 16U));
    } else {
        port->BSRR = (1U << (uint32_t)pin);
    }
    return HAL_OK;
}
