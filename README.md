# STM32F723E Discovery Kit External Memory Tests

These are a few bare-metal examples demonstrating how to use the STM32's FMC "Flexible Memory Controller" peripheral to transparently map external memories to internal memory space. They're meant to go with a companion blog post discussing the FMC peripheral.

It uses an STM32F732E Discovery Kit board, which includes a PSRAM chip and a 240x240-pixel ST7789 TFT display connected to the FMC peripheral.

* psram\_config: Initialize the board and set up the 512KB of external PSRAM.

* tft\_init: Send commands and data over an "8080" parallel display interface to initialize the display.

* tft\_draw: Place a framebuffer in the external PSRAM, draw to it, and send it to the display using "memory-to-memory" DMA.
