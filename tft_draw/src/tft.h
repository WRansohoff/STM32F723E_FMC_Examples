#ifndef VVC_TFT_H
#define VVC_TFT_H

#include "global.h"

// Memory addresses used for display commands and data.
volatile uint16_t* tft_cmd;
volatile uint16_t* tft_dat;

// Macros to read / write TFT commands / data.
// These use memory barriers since the display is an external memory.
#define TFT_CMD( x ) \
  *tft_cmd = x; __DSB(); __ISB();
#define TFT_WR( x ) \
  *tft_dat = x; __DSB(); __ISB();
// Usage: `uint8_t var = TFT_RD();`
#define TFT_RD() \
  *tft_dat; __DSB(); __ISB();

// Helper method to send initialization commands to the TFT display.
void tft_init( void );
// Helper method to set the display drawing area to cover
// the entire 240x240-pixel screen.
void tft_draw_fullscreen();

// TFT drawing functions. These draw to the framebuffer, so they
// will not be reflected on the display until the next refresh.
void tft_px( uint16_t* fb, int x, int y, uint16_t col );
void tft_hline( uint16_t* fb, int x, int y, int len, uint16_t col );
void tft_vline( uint16_t* fb, int x, int y, int len, uint16_t col );
void tft_rect( uint16_t* fb, int x, int y, int w, int h,
               int outline, uint16_t col );
void tft_glyph( uint16_t* fb, int x, int y, uint32_t w0, uint32_t w1,
                uint16_t col, int size );
void tft_char( uint16_t* fb, int x, int y, char c,
               uint16_t col, int size );
void tft_text( uint16_t* fb, int x, int y, const char* str,
               uint16_t col, int size );

#endif
