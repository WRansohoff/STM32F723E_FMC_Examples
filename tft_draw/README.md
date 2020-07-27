# TFT Drawing Example

This STM32F723E Discovery Kit example demonstrates how to draw to a display using an "8080" parallel display interface.

It initializes the FMC peripheral to talk to the display and external PSRAM, places a framebuffer in PSRAM, and sends that framebuffer to the display using DMA.

The main loop also calculates how many frames per second the display is being refreshed at, and prints that value to the display.
