/**
 * Source file containing implementation for simple tests for the gpio_driver HAL
 * as of now, they are not automated and don't use a testing framework, just visual inspection
 * 
 * Written by Ryan Wong
 */

 /*
Still to test:
- toggle
- write port
- read pin
- read port
- lock pins
- AF capabilities
*/

 #include "test/gpio_driver_test.h"
 #include "drivers/gpio_driver.h"

void GPIO_test_init() {
    GPIO_Init_TypeDef init;
    init.mode = GPIO_MODE_OUTPUT;
    init.otype = GPIO_OTYPE_PP;
    init.ospeed = GPIO_OSPEED_LOW;
    init.pupd = GPIO_PUPD_NONE;
    init.afx = GPIO_AF0;
    init.init_out_state = PIN_RESET;

    GPIO_enable_clock(GPIOA);
    GPIO_init(GPIOA, GPIO_PIN_5, &init);
}

void GPIO_test() {
    GPIO_write_pin(GPIOA, GPIO_PIN_5, PIN_SET);
    // delay (not implemented yet)
}