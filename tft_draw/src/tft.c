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

// Draw one pixel to the TFT framebuffer.
void tft_px( uint16_t* fb, int x, int y, uint16_t col ) {
  if ( ( x < 0 ) | ( x > 239 ) |
       ( y < 0 ) | ( y > 239 ) ) { return; }
  fb[ x + ( y * 240 ) ] = col;
}

// Draw a horizontal line to the TFT framebuffer.
void tft_hline( uint16_t* fb, int x, int y, int l, uint16_t col ) {
  if ( ( x < 0 ) | ( x > 239 ) |
       ( y < 0 ) | ( y > 239 ) ) { return; }
  int len = l;
  if ( ( x + l ) > 239 ) { len = 240 - x; }
  for ( int xp = x; xp < ( x + len ); ++xp ) {
    fb[ xp + ( y * 240 ) ] = col;
  }
}

// Draw a vertical line to the TFT framebuffer.
void tft_vline( uint16_t* fb, int x, int y, int l, uint16_t col ) {
  if ( ( x < 0 ) | ( x > 239 ) |
       ( y < 0 ) | ( y > 239 ) ) { return; }
  int len = l;
  if ( ( y + l ) > 239 ) { len = 240 - y; }
  for ( int yp = y; yp < ( y + len ); ++yp ) {
    fb[ x + ( yp * 240 ) ] = col;
  }
}

// Draw a rectangle to the TFT framebuffer.
void tft_rect( uint16_t* fb, int x, int y, int w, int h,
               int outline, uint16_t col ) {
  if ( ( x < 0 ) | ( x > 239 ) |
       ( y < 0 ) | ( y > 239 ) ) { return; }
  int wd = w;
  int ht = h;
  if ( ( x + w ) > 239 ) { wd = 240 - x; }
  if ( ( y + h ) > 239 ) { ht = 240 - y; }
  if ( outline > 0 ) {
    // Draw a rectangle outline.
    int op;
    for ( op = y; op < ( y + outline ); ++op ) {
      tft_hline( fb, x, op, wd, col );
    }
    for ( op = ( y + ht - 1 ); op > ( y + ht - 1 - outline ); --op ) {
      tft_hline( fb, x, op, wd, col );
    }
    for ( op = x; op < ( x + outline ); ++op ) {
      tft_vline( fb, op, y, ht, col );
    }
    for ( op = ( x + wd - 1 ); op > ( x + wd - 1 - outline ); --op ) {
      tft_vline( fb, op, y, ht, col );
    }
  }
  else {
    // Draw a filled rectangle.
    if ( wd > ht ) {
      // Draw fewer horizontal lines than vertical ones.
      for ( int yp = y; yp < ( y + ht ); ++yp ) {
        tft_hline( fb, x, yp, wd, col );
      }
    }
    else {
      // Draw fewer (or ==) vertical lines than horizontal ones.
      for ( int xp = x; xp < ( x + wd ); ++xp ) {
        tft_vline( fb, xp, y, ht, col );
      }
    }
  }
}

// Draw a 6x8 character glyph, defined across 48 bits of data.
// Only 'foreground' pixels are drawn.
void tft_glyph( uint16_t* fb, int x, int y, uint32_t w0, uint32_t w1,
                uint16_t col, int size ) {
  if ( ( x < 0 ) | ( x > 239 ) |
       ( y < 0 ) | ( y > 239 ) ) { return; }
  int px_incr = size;
  int line_h = 8 * size;
  int cur_x = x;
  int cur_y = y;
  for ( int wi = 31; wi >= 0; --wi ) {
    for ( int cx = cur_x; cx < ( cur_x + px_incr ); ++cx ) {
      for ( int cy = cur_y; cy < ( cur_y + px_incr ); ++cy ) {
        if ( w0 & ( 1 << wi ) ) {
          tft_px( fb, cx, cy, col );
        }
      }
    }
    cur_y += px_incr;
    if ( cur_y == ( y + line_h ) ) {
      cur_y = y;
      cur_x += px_incr;
    }
  }
  for ( int wi = 15; wi >= 0; --wi ) {
    for ( int cx = cur_x; cx < ( cur_x + px_incr ); ++cx ) {
      for ( int cy = cur_y; cy < ( cur_y + px_incr ); ++cy ) {
        if ( w1 & ( 1 << wi ) ) {
          tft_px( fb, cx, cy, col );
        }
      }
    }
    cur_y += px_incr;
    if ( cur_y == ( y + line_h ) ) {
      cur_y = y;
      cur_x += px_incr;
    }
  }
}

// Draw a character to the TFT framebuffer using the appropriate
// 48 bits of font data. See `global.h` for the font data.
void tft_char( uint16_t* fb, int x, int y, char c,
               uint16_t col, int size ) {
  if ( ( x < 0 ) | ( x > 239 ) |
       ( y < 0 ) | ( y > 239 ) ) { return; }
  uint32_t w0, w1;
  if ( c == 'A' ) {
    w0 = CH_A0;
    w1 = CH_A1B1 >> 16;
  }
  else if ( c == 'B' ) {
    w0 = CH_B0;
    w1 = CH_A1B1 & 0x0000FFFF;
  }
  else if ( c == 'C' ) {
    w0 = CH_C0;
    w1 = CH_C1D1 >> 16;
  }
  else if ( c == 'D' ) {
    w0 = CH_D0;
    w1 = CH_C1D1 & 0x0000FFFF;
  }
  else if ( c == 'E' ) {
    w0 = CH_E0;
    w1 = CH_E1F1 >> 16;
  }
  else if ( c == 'F' ) {
    w0 = CH_F0;
    w1 = CH_E1F1 & 0x0000FFFF;
  }
  else if ( c == 'G' ) {
    w0 = CH_G0;
    w1 = CH_G1H1 >> 16;
  }
  else if ( c == 'H' ) {
    w0 = CH_H0;
    w1 = CH_G1H1 & 0x0000FFFF;
  }
  else if ( c == 'I' ) {
    w0 = CH_I0;
    w1 = CH_I1J1 >> 16;
  }
  else if ( c == 'J' ) {
    w0 = CH_J0;
    w1 = CH_I1J1 & 0x0000FFFF;
  }
  else if ( c == 'K' ) {
    w0 = CH_K0;
    w1 = CH_K1L1 >> 16;
  }
  else if ( c == 'L' ) {
    w0 = CH_L0;
    w1 = CH_K1L1 & 0x0000FFFF;
  }
  else if ( c == 'M' ) {
    w0 = CH_M0;
    w1 = CH_M1N1 >> 16;
  }
  else if ( c == 'N' ) {
    w0 = CH_N0;
    w1 = CH_M1N1 & 0x0000FFFF;
  }
  else if ( c == 'O' ) {
    w0 = CH_O0;
    w1 = CH_O1P1 >> 16;
  }
  else if ( c == 'P' ) {
    w0 = CH_P0;
    w1 = CH_O1P1 & 0x0000FFFF;
  }
  else if ( c == 'Q' ) {
    w0 = CH_Q0;
    w1 = CH_Q1R1 >> 16;
  }
  else if ( c == 'R' ) {
    w0 = CH_R0;
    w1 = CH_Q1R1 & 0x0000FFFF;
  }
  else if ( c == 'S' ) {
    w0 = CH_S0;
    w1 = CH_S1T1 >> 16;
  }
  else if ( c == 'T' ) {
    w0 = CH_T0;
    w1 = CH_S1T1 & 0x0000FFFF;
  }
  else if ( c == 'U' ) {
    w0 = CH_U0;
    w1 = CH_U1V1 >> 16;
  }
  else if ( c == 'V' ) {
    w0 = CH_V0;
    w1 = CH_U1V1 & 0x0000FFFF;
  }
  else if ( c == 'W' ) {
    w0 = CH_W0;
    w1 = CH_W1X1 >> 16;
  }
  else if ( c == 'X' ) {
    w0 = CH_X0;
    w1 = CH_W1X1 & 0x0000FFFF;
  }
  else if ( c == 'Y' ) {
    w0 = CH_Y0;
    w1 = CH_Y1Z1 >> 16;
  }
  else if ( c == 'Z' ) {
    w0 = CH_Z0;
    w1 = CH_Y1Z1 & 0x0000FFFF;
  }
  else if ( c == 'a' ) {
    w0 = CH_a0;
    w1 = CH_a1b1 >> 16;
  }
  else if ( c == 'b' ) {
    w0 = CH_b0;
    w1 = CH_a1b1 & 0x0000FFFF;
  }
  else if ( c == 'c' ) {
    w0 = CH_c0;
    w1 = CH_c1d1 >> 16;
  }
  else if ( c == 'd' ) {
    w0 = CH_d0;
    w1 = CH_c1d1 & 0x0000FFFF;
  }
  else if ( c == 'e' ) {
    w0 = CH_e0;
    w1 = CH_e1f1 >> 16;
  }
  else if ( c == 'f' ) {
    w0 = CH_f0;
    w1 = CH_e1f1 & 0x0000FFFF;
  }
  else if ( c == 'g' ) {
    w0 = CH_g0;
    w1 = CH_g1h1 >> 16;
  }
  else if ( c == 'h' ) {
    w0 = CH_h0;
    w1 = CH_g1h1 & 0x0000FFFF;
  }
  else if ( c == 'i' ) {
    w0 = CH_i0;
    w1 = CH_i1j1 >> 16;
  }
  else if ( c == 'j' ) {
    w0 = CH_j0;
    w1 = CH_i1j1 & 0x0000FFFF;
  }
  else if ( c == 'k' ) {
    w0 = CH_k0;
    w1 = CH_k1l1 >> 16;
  }
  else if ( c == 'l' ) {
    w0 = CH_l0;
    w1 = CH_k1l1 & 0x0000FFFF;
  }
  else if ( c == 'm' ) {
    w0 = CH_m0;
    w1 = CH_m1n1 >> 16;
  }
  else if ( c == 'n' ) {
    w0 = CH_n0;
    w1 = CH_m1n1 & 0x0000FFFF;
  }
  else if ( c == 'o' ) {
    w0 = CH_o0;
    w1 = CH_o1p1 >> 16;
  }
  else if ( c == 'p' ) {
    w0 = CH_p0;
    w1 = CH_o1p1 & 0x0000FFFF;
  }
  else if ( c == 'q' ) {
    w0 = CH_q0;
    w1 = CH_q1r1 >> 16;
  }
  else if ( c == 'r' ) {
    w0 = CH_r0;
    w1 = CH_q1r1 & 0x0000FFFF;
  }
  else if ( c == 's' ) {
    w0 = CH_s0;
    w1 = CH_s1t1 >> 16;
  }
  else if ( c == 't' ) {
    w0 = CH_t0;
    w1 = CH_s1t1 & 0x0000FFFF;
  }
  else if ( c == 'u' ) {
    w0 = CH_u0;
    w1 = CH_u1v1 >> 16;
  }
  else if ( c == 'v' ) {
    w0 = CH_v0;
    w1 = CH_u1v1 & 0x0000FFFF;
  }
  else if ( c == 'w' ) {
    w0 = CH_w0;
    w1 = CH_w1x1 >> 16;
  }
  else if ( c == 'x' ) {
    w0 = CH_x0;
    w1 = CH_w1x1 & 0x0000FFFF;
  }
  else if ( c == 'y' ) {
    w0 = CH_y0;
    w1 = CH_y1z1 >> 16;
  }
  else if ( c == 'z' ) {
    w0 = CH_z0;
    w1 = CH_y1z1 & 0x0000FFFF;
  }
  else if ( c == '0' ) {
    w0 = CH_00;
    w1 = CH_0111 >> 16;
  }
  else if ( c == '1' ) {
    w0 = CH_10;
    w1 = CH_0111 & 0x0000FFFF;
  }
  else if ( c == '2' ) {
    w0 = CH_20;
    w1 = CH_2131 >> 16;
  }
  else if ( c == '3' ) {
    w0 = CH_30;
    w1 = CH_2131 & 0x0000FFFF;
  }
  else if ( c == '4' ) {
    w0 = CH_40;
    w1 = CH_4151 >> 16;
  }
  else if ( c == '5' ) {
    w0 = CH_50;
    w1 = CH_4151 & 0x0000FFFF;
  }
  else if ( c == '6' ) {
    w0 = CH_60;
    w1 = CH_6171 >> 16;
  }
  else if ( c == '7' ) {
    w0 = CH_70;
    w1 = CH_6171 & 0x0000FFFF;
  }
  else if ( c == '8' ) {
    w0 = CH_80;
    w1 = CH_8191 >> 16;
  }
  else if ( c == '9' ) {
    w0 = CH_90;
    w1 = CH_8191 & 0x0000FFFF;
  }
  else if ( c == ':' ) {
    w0 = CH_col0;
    w1 = CH_col1per1 >> 16;
  }
  else if ( c == '.' ) {
    w0 = CH_per0;
    w1 = CH_col1per1 & 0x0000FFFF;
  }
  else if ( c == '!' ) {
    w0 = CH_exc0;
    w1 = CH_exc1fws1 >> 16;
  }
  else if ( c == '/' ) {
    w0 = CH_fws0;
    w1 = CH_exc1fws1 & 0x0000FFFF;
  }
  else if ( c == '-' ) {
    w0 = CH_hyp0;
    w1 = CH_hyp1pls1 >> 16;
  }
  else if ( c == '+' ) {
    w0 = CH_pls0;
    w1 = CH_hyp1pls1 & 0x0000FFFF;
  }
  else if ( c == '<' ) {
    w0 = CH_lct0;
    w1 = CH_lct1rct1 >> 16;
  }
  else if ( c == '>' ) {
    w0 = CH_rct0;
    w1 = CH_lct1rct1 & 0x0000FFFF;
  }
  else {
    w0 = 0;
    w1 = 0;
  }
  tft_glyph( fb, x, y, w0, w1, col, size );
}

// Draw a null-terminated string of text to the TFT framebuffer.
// Text wrapping is not implemented.
// TODO: Take a 'string length' parameter instead?
void tft_text( uint16_t* fb, int x, int y, const char* str,
               uint16_t col, int size ) {
  if ( ( x < 0 ) | ( x > 239 ) |
       ( y < 0 ) | ( y > 239 ) ) { return; }
  int i = 0;
  int offset = 0;
  while( str[ i ] != '\0' ) {
    tft_char( fb, ( x + offset ), y, str[ i ], col, size );
    offset += 6 * size;
    ++i;
  }
}
