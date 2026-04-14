
create_clock -name clk -period 10 [get_ports clk]

set_input_delay 1.0 -clock clk [all_inputs]
set_input_delay 0.0 -clock clk [get_ports clk]

set_output_delay 1.0 -clock clk [all_outputs]

set_driving_cell -lib_cell sky130_fd_sc_hd__inv_1 [all_inputs]

set_load 0.5 [all_outputs]

