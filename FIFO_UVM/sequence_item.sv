//object class

class fifo_sequence_item extends uvm_sequence_item;
  
  `uvm_object_utils(fifo_sequence_item)
  
  //instantiation
    
  rand logic wrst_n, rrst_n;
  rand logic w_en, r_en;
  rand logic [7:0] data_in;
  logic [7:0] data_out; //output
  logic full, empty; //output
  
  
  function new(string name = "fifo_sequence_item");
    
    super.new(name);
    
    
  endfunction
  
endclass

