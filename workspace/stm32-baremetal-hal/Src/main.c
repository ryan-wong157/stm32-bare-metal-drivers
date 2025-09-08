/**
 * Main function containing tests for each bare-metal HAL driver
 *
 * Written by Ryan Wong
 */

#include <stdint.h>

#include "drivers/gpio_driver.h"

int main(void) {
    GPIO_enable_clock(GPIOA);
    GPIO_init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_OSPEED_LOW, GPIO_PUPD_NONE);
    GPIO_write_pin(GPIOA, GPIO_PIN_5, PIN_RESET);
    // MAIN LOOP --------------------------------------------
	for(;;) {
        GPIO_write_pin(GPIOA, GPIO_PIN_5, PIN_SET);
    }
}
