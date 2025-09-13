/*
 * rcc_driver.c
 *
 * implementation file for rcc_driver.h
 *
 *  Written by Ryan Wong
 */

#include <stdint.h>
#include "drivers/rcc_driver.h"

static uint32_t get_prescaler_from_ppre(uint32_t ppre);

// Global variable specifying HCLK frequency
volatile uint32_t HCLK_frequency = HSI_FREQ;


// HAL FUNCTIONS ==============================================================
/**
 * This function uses some interesting bit arithmetic to map the hpre bit values to prescaler values
 * It uses the fact that shifting a number left one bit is equal to multiplying by 2 
 * It first checks what the SYSCLK clock source is, then divides this by the AHB prescaler to get the HCLK value
 */
void update_hclk() {
    uint32_t clk_src = (RCC_CFGR & (0x03U << 2)) >> 2;
    uint32_t hpre = (RCC_CFGR & (0x0FU << 4)) >> 4;
    uint32_t divisor;
    
    if (!(hpre & (0x01U << 3))) {
        // if bit 3 is not set, div = 1
        divisor = 1;
    } else if (hpre & (0x01U << 2)) {
        // if bit 3 and 2 are set, divisor is 64 << (first 2 bits)
        divisor = 64U << (hpre & 0x03U);
    } else {
        // If bit 3 is set but bit 2 is not set, divisor is 2 << (first 2 bits)
        divisor = 2U << (hpre & 0x03U);
    }

    if (clk_src == 0x00) {
        // HSI selected
        HCLK_frequency = HSI_FREQ / divisor;        
    } else if (clk_src == 0x01) {
        // HSE selected
        // do nothing for now because HSE switching is not implemented
    }
}

/**
 * Anything below 0b1000 for div will be treated as div by 1 anyway
 * Also updates SystemCoreClock global variable which specifies HCLK frequency
 */
HAL_Status RCC_set_AHB_prescaler(RCC_AHB_Prescaler div) {
    if (
        div > RCC_AHB_DIV_512
    ) return HAL_ERROR;

    RCC_CFGR &= ~(0xFU << 4);
    RCC_CFGR |= (uint32_t)div << 4;

    update_hclk();
    return HAL_OK;
}

/**
 * Has a check with should prevent the final PCLK1 frequency from exceeding 45MHz
 */
HAL_Status RCC_set_APB1_prescaler(RCC_APB_Prescaler div) {
    if (
        div > RCC_APB_DIV_16
    ) return HAL_ERROR;

    // Check if final PCLK1 frequency will exceed 45MHz
    uint32_t divisor = get_prescaler_from_ppre(div);
    if (HCLK_frequency / divisor > 45000000) {
        return HAL_ERROR;
    }
    // Passes checks, set new prescaler for APB1
    RCC_CFGR &= ~(0x07U << 10);
    RCC_CFGR |= div << 10;

    return HAL_OK;
}

/**
 * Has a check which should prevent PCLK2's frequency from exceeding 90MHz
 */
HAL_Status RCC_set_APB2_prescaler(RCC_APB_Prescaler div) {
    if (
        div > RCC_APB_DIV_16
    ) return HAL_ERROR;

    // Check if final PCLK2 frequency will exceed 90MHz
    uint32_t divisor = get_prescaler_from_ppre(div);
    if (HCLK_frequency / divisor > 90000000) {
        return HAL_ERROR;
    }
    // Passes checks, set new prescaler for APB2
    RCC_CFGR &= ~(0x07U << 13);
    RCC_CFGR |= div << 13;

    return HAL_OK; 
}

// HELPER FUNCTIONS ==============================================================
static uint32_t get_prescaler_from_ppre(uint32_t ppre) {
    if (!((uint32_t)ppre & (0x01U << 2))) {
        // If bit 2 is not set - prescaler will always be 1
        return 1;
    } else {
        // If bit 2 is set - prescaler = 2 << (first 2 bits)
        return 2U << ((uint32_t)ppre & 0x03U);
    }
}