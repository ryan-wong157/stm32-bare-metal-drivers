/*
* rcc_driver.h
*
* Header file for rcc_driver.c
* Contains functions which configure and enable the clock settings and various bus clock prescalers
* For safety, these functions should only be called on STARTUP, before each peripheral's clocks are enabled!
*
*  Written by Ryan Wong
*/

#ifndef RCC_DRIVER_H_
#define RCC_DRIVER_H_

#include "drivers/types.h"

// Globals and Constants =========================================================
// Global variable specifying HCLK frequency in Hz (driving the CPU and SysTick)
extern volatile uint32_t HCLK_frequency;

// HSI is 16MHz for the STM32F4 (dunno if its a cortex M4 default or vendor specific)
#define HSI_FREQ 16000000U


// REGISTERS =====================================================================
#define RCC_BASE 0x40023800U
#define RCC_CFGR *((volatile uint32_t*)(RCC_BASE + 0x08))

// RCC Config Types ==============================================================
typedef enum {
    RCC_AHB_DIV_1 = 0b0000U,
    RCC_AHB_DIV_2 = 0b1000U,
    RCC_AHB_DIV_4 = 0b1001U,
    RCC_AHB_DIV_8 = 0b1010U,
    RCC_AHB_DIV_16 = 0b1011U,
    RCC_AHB_DIV_64 = 0b1100U,
    RCC_AHB_DIV_128 = 0b1101U,
    RCC_AHB_DIV_256 = 0b1110U,
    RCC_AHB_DIV_512 = 0b1111U
} RCC_AHB_Prescaler;

typedef enum {
    RCC_APB_DIV_1 = 0b000U,
    RCC_APB_DIV_2 = 0b100U,
    RCC_APB_DIV_4 = 0b101U,
    RCC_APB_DIV_8 = 0b110U,
    RCC_APB_DIV_16 = 0b111U,
} RCC_APB_Prescaler;


// HAL FUNCTIONS ==============================================================

/**
 * @brief Updates hclk frequency. You should call this anytime you touch SYSCLK or AHB Prescaler!
 * Failure to do so will affect the SysTick configuration and Delay() function
 * 
 */
void update_hclk();

/**
 * @brief Sets the HCLK (AHB clock) prescaler.
 * The clocks are divided with the new prescaler factor from 1 to 16 AHB cycles after HPRE write.
 * CAUTION - the AHB clock frequency must be at least 25MHz when Ethernet is used
 * 
 * @param div - Enum specifying the prescaler
 * @return HAL_Status 
 */
HAL_Status RCC_set_AHB_prescaler(RCC_AHB_Prescaler div);

/**
 * @brief Updates the APB low speed (1) prescaler.
 * CAUTION - the APB1 clock frequency (PCLK1) must NOT exceed 45MHz!
 * This should be enforced by this function, but note it will return HAL_ERROR if attempted
 * 
 * @param div - enum above specifying the prescaler
 * @return HAL_Status 
 */
HAL_Status RCC_set_APB1_prescaler(RCC_APB_Prescaler div);

/**
 * @brief Updates the APB high speed (2) prescaler.
 * CAUTION - the APB2 clock frequency (PCLK2) must NOT exceed 90MHz!
 * This should be enforced by this function
 * 
 * @param div - enum above specifying the prescaler
 * @return HAL_Status 
 */
HAL_Status RCC_set_APB2_prescaler(RCC_APB_Prescaler div);

#endif