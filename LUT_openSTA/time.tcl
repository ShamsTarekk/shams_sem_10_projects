read_verilog Lut_netlist.v

read_liberty /home/asiclab/.ciel/ciel/sky130/versions/0fe599b2afb6708d281543108caf8310912f54af/sky130B/libs.ref/sky130_fd_sc_hd/lib/sky130_fd_sc_hd__tt_025C_1v80.lib

link_design Lut

create_clock -name clk -period 5.6 [get_ports clk]

set_input_delay 2.0 -clock clk [all_inputs]
set_input_delay 0.5 -clock clk [get_ports clk]

set_output_delay 2.0 -clock clk [all_outputs]


set_load 0.5 [all_outputs]

report_checks -path_delay max -format full > setup.rpt

report_wns > time.rpt
report_tns >> time.rpt

report_checks -path_delay min -format full > hold.rpt