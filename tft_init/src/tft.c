#include "tft.h"

// Address bit 0 is used for the 'data / command' pin.
// (That's encoded as bit 1 in 16-bit mode)
volatile uint16_t* tft_cmd = ( volatile uint16_t* )0x64000000;
volatile uint16_t* tft_dat = ( volatile uint16_t* )0x64000002;

// Helper method to send initialization commands to the TFT display.
void tft_init( void ) {
  // Perform a hardware reset by toggling the display's reset pin.
  GPIOH->ODR &= ~( 1 << 7 );
  delay_ms( 100 );
  GPIOH->ODR |=  ( 1 << 7 );

  // Send 'software reset' command and wait for the display to reset.
  *tft_cmd = 0x01;
  delay_ms( 100 );

  // Send 'sleep out' command and wait for the display to wake up.
  *tft_cmd = 0x11;
  delay_ms( 100 );

  // Send 'display on' command.
  // Use memory barriers when delays aren't needed to ensure that
  // the external memory operation completes before proceeding.
  TFT_CMD( 0x29 );
  // Color mode: 16bpp.
  TFT_CMD( 0x3A );
  TFT_WR( 0x55 );
  // Display inversion "on". When this is off, 0x0000 = white...
  TFT_CMD( 0x21 );

  // Set the display drawing area.
  tft_draw_fullscreen();
}

// Helper method to set the display drawing area to cover
// the entire 240x240-pixel screen.
void tft_draw_fullscreen() {
  // Send the memory access commands.
  TFT_CMD( 0x2A );
  TFT_WR( 0x00 );
  TFT_WR( 0x00 );
  TFT_WR( 239 & 0xFF );
  TFT_WR( ( 239 >> 8 ) & 0xFF );
  TFT_CMD( 0x2B );
  TFT_WR( 0x00 );
  TFT_WR( 0x00 );
  TFT_WR( 239 & 0xFF );
  TFT_WR( ( 239 >> 8 ) & 0xFF );
  TFT_CMD( 0x2C );
}
