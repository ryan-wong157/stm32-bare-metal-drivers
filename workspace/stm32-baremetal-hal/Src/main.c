/**
 * Main function containing tests for each bare-metal HAL driver
 *
 * Written by Ryan Wong
 */

#include <stdint.h>

#include "drivers/gpio_driver.h"

int main(void) {
    GPIO_init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_OSPEED_LOW, GPIO_PUPD_NONE);

    // MAIN LOOP --------------------------------------------
	for(;;) {

    }
}
