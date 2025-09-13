/*
 * rcc_driver.c
 *
 * implementation file for rcc_driver.h
 *
 *  Written by Ryan Wong
 */

#include <stdint.h>
#include "drivers/rcc_driver.h"

// Global variable specifying HCLK frequency
uint32_t HCLK_frequency = HSI_FREQ;

void update_hclk() {
    // read clock source, read AHB prescaler, update value
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

    return HAL_OK;
}