#include "main.h"

// Reset handler: set the stack pointer and branch to main().
__attribute__( ( naked ) ) void reset_handler( void ) {
  // Set the stack pointer to the 'end of stack' value.
  __asm__( "LDR r0, =_estack\n\t"
           "MOV sp, r0" );
  // Branch to main().
  __asm__( "B main" );
}

/**
 * Main program.
 */
int main( void ) {
  // Copy initialized data from .sidata (Flash) to .data (RAM)
  memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
  // Clear the .bss section in RAM.
  memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

  // Enable floating-point unit.
  SCB->CPACR    |=  ( 0xF << 20 );

  // Set clock speed to 216MHz (each tick is a bit less than 5ns)
  // PLL out = ( 16MHz * ( N / M ) / P ). P = 2, N = 54, M = 2.
  FLASH->ACR   |=  ( 7 << FLASH_ACR_LATENCY_Pos );
  RCC->PLLCFGR &= ~( RCC_PLLCFGR_PLLN |
                     RCC_PLLCFGR_PLLM );
  RCC->PLLCFGR |=  ( ( 54 << RCC_PLLCFGR_PLLN_Pos ) |
                     ( 2 << RCC_PLLCFGR_PLLM_Pos ) );
  RCC->CR      |=  ( RCC_CR_PLLON );
  while ( !( RCC->CR & RCC_CR_PLLRDY ) ) {};
  RCC->CFGR    |=  ( 2 << RCC_CFGR_SW_Pos );
  while ( ( RCC->CFGR & RCC_CFGR_SWS ) != ( 2 << RCC_CFGR_SWS_Pos ) ) {};
  SystemCoreClock = 216000000;

  // Setup the SysTick peripheral to generate 1ms ticks.
  SysTick_Config( SystemCoreClock / 1000 );

  // Enable peripheral clocks: GPIOA-H, TIM5, USART6, FMC, DMA2.
  RCC->AHB1ENR |=  ( RCC_AHB1ENR_GPIOAEN |
                     RCC_AHB1ENR_GPIOBEN |
                     RCC_AHB1ENR_GPIOCEN |
                     RCC_AHB1ENR_GPIODEN |
                     RCC_AHB1ENR_GPIOEEN |
                     RCC_AHB1ENR_GPIOFEN |
                     RCC_AHB1ENR_GPIOGEN |
                     RCC_AHB1ENR_GPIOHEN |
                     RCC_AHB1ENR_DMA2EN );
  RCC->AHB3ENR |=  ( RCC_AHB3ENR_FMCEN );
  RCC->APB1ENR |=  ( RCC_APB1ENR_TIM5EN );
  RCC->APB2ENR |=  ( RCC_APB2ENR_USART6EN );

  // Initialize GPIO pins for the appropriate peripherals.
  // C6, C7: USART6 TX/RX.
  gpio_qconfig( GPIOC, 6, 2, 2, 8 );
  gpio_qconfig( GPIOC, 7, 2, 2, 8 );
  // H7: Display reset, H11: Backlight brightness control.
  gpio_qconfig( GPIOH, 7, 1, 0, 0 );
  gpio_qconfig( GPIOH, 11, 2, 0, 2 );
  // FMC Pins: B7, D0, D1, D4, D5, D7, D8, D9, D10, D11, D12, D14,
  // D15, E0, E1, E7, E8, E9, E10, E11, E12, E13, E14, E15, F0, F1,
  // F2, F3, F4, F5, F12, F13, F14, F15, G0, G1, G2, G3, G4, G5, G9
  gpio_qconfig( GPIOB, 7, 2, 3, 12 );
  gpio_qconfig( GPIOD, 0, 2, 3, 12 );
  gpio_qconfig( GPIOD, 1, 2, 3, 12 );
  gpio_qconfig( GPIOD, 4, 2, 3, 12 );
  gpio_qconfig( GPIOD, 5, 2, 3, 12 );
  gpio_qconfig( GPIOD, 7, 2, 3, 12 );
  gpio_qconfig( GPIOD, 8, 2, 3, 12 );
  gpio_qconfig( GPIOD, 9, 2, 3, 12 );
  gpio_qconfig( GPIOD, 10, 2, 3, 12 );
  gpio_qconfig( GPIOD, 11, 2, 3, 12 );
  gpio_qconfig( GPIOD, 12, 2, 3, 12 );
  gpio_qconfig( GPIOD, 14, 2, 3, 12 );
  gpio_qconfig( GPIOD, 15, 2, 3, 12 );
  gpio_qconfig( GPIOE, 0, 2, 3, 12 );
  gpio_qconfig( GPIOE, 1, 2, 3, 12 );
  gpio_qconfig( GPIOE, 7, 2, 3, 12 );
  gpio_qconfig( GPIOE, 8, 2, 3, 12 );
  gpio_qconfig( GPIOE, 9, 2, 3, 12 );
  gpio_qconfig( GPIOE, 10, 2, 3, 12 );
  gpio_qconfig( GPIOE, 11, 2, 3, 12 );
  gpio_qconfig( GPIOE, 12, 2, 3, 12 );
  gpio_qconfig( GPIOE, 13, 2, 3, 12 );
  gpio_qconfig( GPIOE, 14, 2, 3, 12 );
  gpio_qconfig( GPIOE, 15, 2, 3, 12 );
  gpio_qconfig( GPIOF, 0, 2, 3, 12 );
  gpio_qconfig( GPIOF, 1, 2, 3, 12 );
  gpio_qconfig( GPIOF, 2, 2, 3, 12 );
  gpio_qconfig( GPIOF, 3, 2, 3, 12 );
  gpio_qconfig( GPIOF, 4, 2, 3, 12 );
  gpio_qconfig( GPIOF, 5, 2, 3, 12 );
  gpio_qconfig( GPIOF, 12, 2, 3, 12 );
  gpio_qconfig( GPIOF, 13, 2, 3, 12 );
  gpio_qconfig( GPIOF, 14, 2, 3, 12 );
  gpio_qconfig( GPIOF, 15, 2, 3, 12 );
  gpio_qconfig( GPIOG, 0, 2, 3, 12 );
  gpio_qconfig( GPIOG, 1, 2, 3, 12 );
  gpio_qconfig( GPIOG, 2, 2, 3, 12 );
  gpio_qconfig( GPIOG, 3, 2, 3, 12 );
  gpio_qconfig( GPIOG, 4, 2, 3, 12 );
  gpio_qconfig( GPIOG, 5, 2, 3, 12 );
  gpio_qconfig( GPIOG, 9, 2, 3, 12 );

  // Setup USART6 for 115200-baud TX.
  USART6->BRR  =  ( SystemCoreClock / 115200 );
  USART6->CR1 |=  ( USART_CR1_UE | USART_CR1_TE );

  // Initialize the memory controller for the connected PSRAM.
  // The 512KB of addressable RAM easily fits in the 64MB bank 1.
  // Bank 1 is enabled (but configured for NOR Flash) at reset.
  // So clear the 'flash enable' bit and set it to PSRAM mode.
  FMC_Bank1->BTCR[ 0 ] &= ~( FMC_BCR1_FACCEN |
                             FMC_BCR1_MTYP );
  FMC_Bank1->BTCR[ 0 ] |=  ( 1 << FMC_BCR1_MTYP_Pos );
  // Set access timings: 5ns bus turnaround, 55ns R/W cycle.
  FMC_Bank1->BTCR[ 1 ] &= ~( FMC_BTR1_BUSTURN |
                             FMC_BTR1_DATAST |
                             FMC_BTR1_ADDHLD |
                             FMC_BTR1_ADDSET );
  FMC_Bank1->BTCR[ 1 ] |=  ( ( 1 << FMC_BTR1_BUSTURN_Pos ) |
                             ( 11 << FMC_BTR1_DATAST_Pos ) );

  // Test external RAM reads and writes.
  // Note: Uninitialized RAM can hold pseudorandom values.
  //uint32_t* psram  = ( uint32_t* )0x60000000;
  //uint16_t* psramh = ( uint16_t* )0x60000000;
  //uint8_t*  psramb = ( uint8_t*  )0x60000000;

  // Initialize the memory controller for the connected TFT display.
  // It is connected to the 'NE2' signal, so use bank 1, section 2.
  FMC_Bank1->BTCR[ 2 ] &= ~( FMC_BCR2_FACCEN |
                             FMC_BCR2_MTYP );
  // Timings: 10ns address hold, 65ns write cycle.
  FMC_Bank1->BTCR[ 3 ] &= ~( FMC_BTR2_BUSTURN |
                             FMC_BTR2_DATAST |
                             FMC_BTR2_ADDHLD |
                             FMC_BTR2_ADDSET );
  FMC_Bank1->BTCR[ 3 ] |=  ( ( 13 << FMC_BTR2_DATAST_Pos ) |
                             ( 2 << FMC_BTR2_ADDHLD_Pos ) );
  FMC_Bank1->BTCR[ 2 ] |=  ( 1 << FMC_BCR2_MTYP_Pos |
                             FMC_BCR2_MBKEN );

  // Send display initialization commands.
  tft_init();

  // Turn on the display backlight @10% brightness, 30KHz PWM.
  uint32_t psc = 0;
  uint32_t arr = SystemCoreClock / 30000;
  if ( arr > 0xFFFF ) {
    psc = ( arr / 0xFFFF ) + 1;
    arr = arr / psc;
  }
  uint16_t ccr = ( uint16_t )( arr * 0.1 );
  TIM5->PSC    = psc;
  TIM5->ARR    = arr;
  TIM5->CCR2   = ccr;
  TIM5->CCER  |=  ( TIM_CCER_CC2E );
  TIM5->CCMR1 &= ~( TIM_CCMR1_OC2M );
  TIM5->CCMR1 |=  ( 0x6 << TIM_CCMR1_OC2M_Pos );
  TIM5->EGR   |=  ( TIM_EGR_UG );
  TIM5->CR1   |=  ( TIM_CR1_CEN );

  // Done; infinite loop.
  while( 1 ) {};
  return 0; // lol
}

// SysTick interrupt handler: increment the global 'systick' value.
void SysTick_IRQn_handler( void ) {
  ++systick;
}
