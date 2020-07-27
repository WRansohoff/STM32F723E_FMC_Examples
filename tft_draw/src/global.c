#include "global.h"

uint32_t SystemCoreClock = 16000000;
volatile uint32_t systick = 0;

// System call to support standard library print functions.
int _write( int handle, char* data, int size ) {
  int count = size;
  while( count-- ) {
    while( !( USART6->ISR & USART_ISR_TXE ) ) {};
    USART6->TDR = *data++;
  }
  return size;
}

// Helper method to configure GPIO pins.
void gpio_qconfig( GPIO_TypeDef* GPIOx, int pin,
                   int mode, int ospeed, int af ) {
  GPIOx->MODER      &= ~( 0x3    << ( pin * 2 ) );
  GPIOx->MODER      |=  ( mode   << ( pin * 2 ) );
  GPIOx->OSPEEDR    &= ~( 0x3    << ( pin * 2 ) );
  GPIOx->OSPEEDR    |=  ( ospeed << ( pin * 2 ) );
  if ( pin <= 7 ) {
    GPIOx->AFR[ 0 ] &= ~( 0xF    << ( pin * 4 ) );
    GPIOx->AFR[ 0 ] |=  ( af     << ( pin * 4 ) );
  }
  else {
    GPIOx->AFR[ 1 ] &= ~( 0xF    << ( ( pin - 8 ) * 4 ) );
    GPIOx->AFR[ 1 ] |=  ( af     << ( ( pin - 8 ) * 4 ) );
  }
}

// Delay for a specified number of milliseconds.
// TODO: Prevent rollover bug on the 'systick' value.
void delay_ms( uint32_t ms ) {
  // Calculate the 'end of delay' tick value, then wait for it.
  uint32_t next = systick + ms;
  while ( systick < next ) { __WFI(); }
}
