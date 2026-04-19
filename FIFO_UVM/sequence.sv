//object class


class fifo_base_sequence extends uvm_sequence;
  
  `uvm_object_utils(fifo_base_sequence)
  
  fifo_sequence_item reset_pkt;
  
  
  function new(string name = "fifo_base_sequence");
    
    super.new(name);
    `uvm_info("Base_sequence", "Inside constructor!", UVM_HIGH)
  endfunction
  
  
  task body();
    `uvm_info("Base_sequence", "Inside body task", UVM_HIGH)
    
    reset_pkt = fifo_sequence_item::type_id::create("reset_pkt");
    start_item(reset_pkt);
    
    reset_pkt.randomize() with {wrst_n == 0; rrst_n ==0;};
    finish_item(reset_pkt);
    
    
  endtask
  
  
  
endclass


class fifo_test_sequence extends fifo_base_sequence;
  `uvm_object_utils(fifo_test_sequence)
  
  fifo_sequence_item item;
  
  
  function new(string name = "fifo_test_sequence");
    
    super.new(name);
    `uvm_info("Test_sequence", "Inside constructor!", UVM_HIGH)
  endfunction
  
  
  task body();
    `uvm_info("Test_sequence", "Inside body task", UVM_HIGH)
    repeat(16) begin
      item = fifo_sequence_item::type_id::create("item");
      start_item(item);

      item.randomize() with {wrst_n == 1; rrst_n ==1; w_en == 1; r_en ==0;};
      finish_item(item);
    end
    repeat(100) begin
      item = fifo_sequence_item::type_id::create("item");
      item.w_en = 0;
      item.r_en =1;
      item.rrst_n = 1;
      item.wrst_n = 1;
      item.data_in = 1;
      start_item(item);
      #10;

      //item.randomize() with {wrst_n == 1; rrst_n ==1; w_en == 0; r_en ==1;};
      finish_item(item);
    end    
  endtask
    
  
endclass

