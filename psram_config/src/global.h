#ifndef VVC_GLOBAL_H
#define VVC_GLOBAL_H

// Standard library includes.
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Device header file.
#include "stm32f7xx.h"

uint32_t SystemCoreClock;
volatile uint32_t systick;
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

// System call to enable standard library print functions.
int _write( int handle, char* data, int size );

// Helper method to configure GPIO pins.
void gpio_qconfig( GPIO_TypeDef* GPIOx, int pin,
                   int mode, int ospeed, int af );

#endif
