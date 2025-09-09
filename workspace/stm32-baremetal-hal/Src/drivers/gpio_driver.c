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
HAL_Status GPIO_enable_clock(GPIO_Reg_TypeDef* port) {
    if ((uint32_t)port > (uint32_t)GPIOH || (uint32_t)port < (uint32_t)GPIOA) return HAL_ERROR;

    uint32_t shift = ((uint32_t)port - GPIO_BASE) / 0x400U;
    RCC_AHB1ENR |= 0x01U << shift;
    return HAL_OK;
}

/**
 * I have chosen to use enums for the options for each pin configuration. 
 * This is for readability and ease of use, although they are not strict compile-time type checks
 */
HAL_Status GPIO_init(GPIO_Reg_TypeDef* port, GPIO_Pin pin, const GPIO_Init_TypeDef* init_struct) {
    GPIO_Mode mode = init_struct->mode;
    GPIO_OType otype = init_struct->otype;
    GPIO_OSpeed ospeed = init_struct->ospeed;
    GPIO_Pupd pupd = init_struct->pupd;
    GPIO_AFx af_mask = init_struct->afx;
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

    // Configure AF - depending on which pin, use low/high AF registers only if AF mode!
    if (mode == GPIO_MODE_AF) {
        if ((uint32_t)pin < 8) {
            port->AFRL &= ~(0xF << (pin * 4));
            port->AFRL |= ((uint32_t)af_mask << (pin * 4));
        } else {
            uint32_t new_pin = pin - 8;
            port->AFRH &= ~(0xF << (new_pin * 4));
            port->AFRH |= ((uint32_t)af_mask << (new_pin * 4));
        }
    }

    // Set default output state only if output mode!
    if (mode == GPIO_MODE_OUTPUT) {
        GPIO_write_pin(port, pin, init_struct->init_out_state);
    }

    return HAL_OK;
}

/**
 * Uses BSRR instead of ODR for atomic writes so it isn't interrupted by any ISR
 */
HAL_Status GPIO_write_pin(GPIO_Reg_TypeDef* port, GPIO_Pin pin, PIN_State val) {
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
HAL_Status GPIO_write_port(GPIO_Reg_TypeDef* port, uint16_t val) {
    if (
        port == NULL
    ) return HAL_ERROR;

    uint16_t inv = ~val;
    port->BSRR = (uint32_t)val | ((uint32_t)(inv) << 16);
    return HAL_OK;
}

/**
 * Instead of XOR toggling the ODR reg, I try to read from it, 
 * and set the relevant bit in BSRR to prevent interrupt race conditions
 */
HAL_Status GPIO_toggle_pin(GPIO_Reg_TypeDef* port, GPIO_Pin pin) {
    if (
        port == NULL ||
        pin > GPIO_PIN_15
    ) return HAL_ERROR;

    if (port->ODR & (0x01U << (uint32_t)pin)) {
        port->BSRR = 0x01U << ((uint32_t)pin + 16U);
    } else {
        port->BSRR = 0x01U << (uint32_t)pin;
    }
    return HAL_OK;
}

/**
 * I made this return the pin value instead of using a pointer to a buffer and filling that out for simplicity
 * The tradeoff is I have to return -1 to indicate error instead of HAL_ERROR
 */
PIN_State GPIO_read_pin(GPIO_Reg_TypeDef* port, GPIO_Pin pin) {
    if (
        port == NULL || 
        pin > GPIO_PIN_15
    ) return -1;

    if (port->IDR & (0x01U << pin)) {
        return PIN_SET;
    } 
    return PIN_RESET;
}

/**
 * Again, I'm minimally checking inputs to prevent null pointer errors.
 * I chose to use a buffer instead of just returning the 16 bit uint so i could return HAL_Status
 */
HAL_Status GPIO_read_port(GPIO_Reg_TypeDef* port, uint16_t* buffer) {
    if (
        port == NULL ||
        buffer == NULL 
    ) return HAL_ERROR;

    // juuuust in case, i mask to remove 16 MSB
    *buffer = (uint16_t)(port->IDR & 0xFFFFU);
    return HAL_OK;
}

/**
 * Using a uint16_t pins bitmask to set the locks as my HAL doesn't currently support multi-pin input 
 * like the STM32 HAL's PIN0 | PIN1...
 */
HAL_Status GPIO_lock_pins(GPIO_Reg_TypeDef* port, uint16_t pins) {
    if (
        port == NULL
    ) return HAL_ERROR;

    // Port LCKR already locked, must reset MCU to disable lock
    if (port->LCKR & (0x01U << 16)) {
        return HAL_ERROR;
    }

    // Overwrite first 15 bits with user entered desired lock bits
    port->LCKR &= ~(0xFFFFU);
    port->LCKR |= (uint32_t)pins;

    // Activate lock (sequence is wr 1 to LCKK, wr 0 in LCKK, wr 1 to LCKK, read from LCKK)
    port->LCKR |= (0x01U << 16);
    port->LCKR &= ~(0x01U << 16);
    port->LCKR |= (0x01U << 16);
    uint32_t temp = port->LCKR;
    (void)temp;

    uint32_t is_locked = port->LCKR & (0x01U << 16);
    if (is_locked) {
        return HAL_OK;
    } 
    return HAL_ERROR;
}