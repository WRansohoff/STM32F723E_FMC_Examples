# PSRAM Configuration Example

This STM32F723E Discovery Kit example demonstrates how to map an external PSRAM chip to internal memory space using the FMC peripheral.

It sets the chip's clock speed to 216MHz, then configures the FMC peripheral with timing values from the IS66WV51216 datasheet:

* Bus turnaround: 5ns
* Address setup: 0ns
* Address hold: 0ns
* Data phase: 55ns

Finally, a couple of values are written to the address where the external RAM should be mapped to, then read back and printed over the board's serial connection.
