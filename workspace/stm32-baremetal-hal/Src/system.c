// Contains SystemInit() implementation called in startup_stm32f446retx.s startup file
#include <stdint.h>

#define FPU_CPACR (*(volatile uint32_t*)0xE000ED88)

void SystemInit(void) {
	// Enable FPU in the coprocessor access control register (set bits 20-23)
	FPU_CPACR |= (3UL << 20) | (3UL << 22);
}

