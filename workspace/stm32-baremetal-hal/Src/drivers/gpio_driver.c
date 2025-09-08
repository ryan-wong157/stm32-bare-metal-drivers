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
 * I have chosen not to use typed enums for mode, otype, ospeed... configuration settings.
 * Although this could be changed in the future (from uint32_t to perhaps GPIO_Otype_TypeDef or something). 
 */
HAL_Status GPIO_init(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd) {
    if (
        port == NULL ||
        pin > GPIO_PIN_15 || 
        mode > GPIO_MODE_ANALOG || 
        otype > GPIO_OTYPE_OD || 
        ospeed > GPIO_OSPEED_HIGH ||
        pupd > GPIO_PUPD_PD
    ) return HAL_ERROR;

    // Clear existing bits, then configure MODER
    port->MODER &= ~(0x03 << (pin * 2));
    port->MODER |= (mode << (pin * 2));

    // Configure OTYPE
    port->OTYPER &= ~(0x01 << pin);
    port->OTYPER |= (otype << pin);

    // Configure OSPEED
    port->OSPEEDR &= ~(0x03 << (pin * 2));
    port->OSPEEDR |= (ospeed << (pin * 2));

    // Configure PUPD
    port->PUPDR &= ~(0x03 << (pin * 2));
    port->PUPDR |= (pupd << (pin * 2));

    return HAL_OK;
}

