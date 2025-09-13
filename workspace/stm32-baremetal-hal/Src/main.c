/**
 * Main function containing tests for each bare-metal HAL driver
 *
 * Written by Ryan Wong
 */

#include <stdint.h>
#include "drivers/gpio_driver.h"
#include "test/gpio_driver_test.h"

int main(void) {
    GPIO_test_init();
    // MAIN LOOP --------------------------------------------
	for(;;) {
        GPIO_test();
    }
}