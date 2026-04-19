interface fifo_interface(input logic wclk, input logic rclk);
  
  
  logic wrst_n, rrst_n;
  logic w_en, r_en;
  logic [7:0] data_in;
  logic [7:0] data_out;
  logic full, empty;
  
  
endinterface;
