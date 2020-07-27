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
// Blocking millisecond delay.
void delay_ms( uint32_t ms );

// Define a simple monospace font; each character is 6x8 pixels,
// which comes out to 6 bytes or 3 words for every 2 characters.
#define CH_A0       0x1F688868
#define CH_B0       0xFF898989
#define CH_A1B1     0x1F007600
#define CH_C0       0x7E818181
#define CH_D0       0xFF818181
#define CH_C1D1     0x66007E00
#define CH_E0       0xFF898989
#define CH_F0       0xFF888888
#define CH_E1F1     0x81008000
#define CH_G0       0x7E818989
#define CH_H0       0xFF080808
#define CH_G1H1     0x6E00FF00
#define CH_I0       0x8181FF81
#define CH_J0       0x868181FE
#define CH_I1J1     0x81008000
#define CH_K0       0xFF182442
#define CH_L0       0xFF010101
#define CH_K1L1     0x81000100
#define CH_M0       0xFF403040
#define CH_N0       0xFF601806
#define CH_M1N1     0xFF00FF00
#define CH_O0       0x7E818181
#define CH_P0       0xFF888888
#define CH_O1P1     0x7E007000
#define CH_Q0       0x7E818582
#define CH_R0       0xFF888C8A
#define CH_Q1R1     0x7D007100
#define CH_S0       0x66919989
#define CH_T0       0x8080FF80
#define CH_S1T1     0x66008000
#define CH_U0       0xFE010101
#define CH_V0       0x701C031C
#define CH_U1V1     0xFE00E000
#define CH_W0       0xFE010601
#define CH_X0       0xC3241824
#define CH_W1X1     0xFE00C300
#define CH_Y0       0xE0100F10
#define CH_Z0       0x838599A1
#define CH_Y1Z1     0xE000C100
#define CH_a0       0x06292929
#define CH_b0       0xFF090909
#define CH_a1b1     0x1F000600
#define CH_c0       0x1E212121
#define CH_d0       0x060909FF
#define CH_c1d1     0x12000100
#define CH_e0       0x3E494949
#define CH_f0       0x087F8888
#define CH_e1f1     0x3A006000
#define CH_g0       0x32494949
#define CH_h0       0xFF080808
#define CH_g1h1     0x3E000700
#define CH_i0       0x00004F00
#define CH_j0       0x0006015E
#define CH_i1j1     0x00000000
#define CH_k0       0x00FF1C23
#define CH_l0       0x0000FF00
#define CH_k1l1     0x00000000
#define CH_m0       0x3F101F10
#define CH_n0       0x3F10100F
#define CH_m1n1     0x0F000000
#define CH_o0       0x0E111111
#define CH_p0       0x003F2424
#define CH_o1p1     0x0E001800
#define CH_q0       0x3048487E
#define CH_r0       0x003F1010
#define CH_q1r1     0x01000800
#define CH_s0       0x00324949
#define CH_t0       0x20FE2121
#define CH_s1t1     0x26000200
#define CH_u0       0x3C02023E
#define CH_v0       0x18060106
#define CH_u1v1     0x03001800
#define CH_w0       0x1E010201
#define CH_x0       0x110A040A
#define CH_w1x1     0x1E001100
#define CH_y0       0x3209093E
#define CH_z0       0x11131519
#define CH_y1z1     0x00001100
#define CH_00       0x7EE19987
#define CH_10       0x2141FF01
#define CH_0111     0x7E000100
#define CH_20       0x63878D99
#define CH_30       0x66818989
#define CH_2131     0x71007600
#define CH_40       0xF80808FF
#define CH_50       0xE2919191
#define CH_4151     0x08008E00
#define CH_60       0x7E919191
#define CH_70       0x60838CB0
#define CH_6171     0x4E00C000
#define CH_80       0x6E919191
#define CH_90       0x72898989
#define CH_8191     0x6E007E00
#define CH_col0     0x00002400
#define CH_per0     0x00000002
#define CH_col1per1 0x00000000
#define CH_exc0     0x007A0000
#define CH_fws0     0x00061860
#define CH_exc1fws1 0x00000000
#define CH_hyp0     0x00080808
#define CH_pls0     0x00081C08
#define CH_hyp1pls1 0x00000000
#define CH_lct0     0x00081422
#define CH_rct0     0x00442810
#define CH_lct1rct1 0x00000000

#endif
