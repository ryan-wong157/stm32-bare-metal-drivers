/**
 * Main function containing tests for each bare-metal HAL driver
 *
 * Written by Ryan Wong
 */

#include <stdint.h>

#include "drivers/gpio_driver.h"

void GPIO_test_init();

int main(void) {
    GPIO_test_init();
    GPIO_write_pin(GPIOA, GPIO_PIN_5, PIN_RESET);
    // MAIN LOOP --------------------------------------------
	for(;;) {
        GPIO_write_pin(GPIOA, GPIO_PIN_5, PIN_SET);
    }
}

void GPIO_test_init() {
    GPIO_Init_TypeDef init;
    init.mode = GPIO_MODE_OUTPUT;
    init.otype = GPIO_OTYPE_PP;
    init.ospeed = GPIO_OSPEED_LOW;
    init.pupd = GPIO_PUPD_NONE;

    GPIO_enable_clock(GPIOA);
    GPIO_init(GPIOA, GPIO_PIN_5, &init);
}