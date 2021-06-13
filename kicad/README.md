# Axis Drivers for Mirrors

## Interface
### Power
Each board has a 16-30v DC, single rail input. This is the operating voltage of the output amplifiers.

### Output
At the end of the board is the output terminal block to be connected to one of the yoke's deflection coils.

### Digital Interface
At the other end is a parallel interface header for sending data to the DAC. The DAC operates over a 5v interface and all connections have 10k pullup resistors, meaning that unused inputs can be left unconnected. There is a 12-bit parallel data bus, read/write, chip select, reset, and load inputs.

## Output Drive Design
In order to achieve a differential output, two output amplifiers are used, one the inversion of the other around 0.5 * VCC.

## Known Issues
* The heatsinks currently on the KiCad boards are inadequate for the output amplifiers if running at greater than about 0.5A output current. (Active cooling is required)
