# Newton-Raphson 16-bit Divider Implementation

## Project Overview
This project implements a 16-bit divider using the Newton-Raphson algorithm. The design is synthesized and evaluated across multiple target technologies including FPGA, SkyWater 130nm, and GF180MCU process nodes.


## Design Description
The Newton-Raphson divider uses an iterative algorithm to perform division operations. The implementation consists of:
- `divider.v` - Core division logic
- `iteration.v` - Iteration control and Newton-Raphson approximation

## Tools Used
- **Icarus Verilog** - Simulation and verification
- **GTKWave** - Waveform viewing and analysis
- **Yosys** - Synthesis tool for all target platforms
