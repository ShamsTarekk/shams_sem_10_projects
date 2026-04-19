class fifo_driver extends uvm_driver#(fifo_sequence_item);
  
  `uvm_component_utils(fifo_driver)
  
  
  //declare handle of virtual interface for driver
  virtual fifo_interface vif;
  fifo_sequence_item item;
  
  function new(string name = "fifo_driver", uvm_component parent);
    super.new(name, parent);
    `uvm_info("Driver_class", "Inside constructor!", UVM_HIGH)
    
  endfunction
  
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    `uvm_info("Driver_class", "Inside build phase!", UVM_HIGH)
    //get the virtual interface
    if(!(uvm_config_db #(virtual fifo_interface)::get(this, "*", "vif", vif))) begin
      `uvm_error("Driver_class", "Failed to get vif from uvm config database")
    end
  endfunction

  function void connect_phase(uvm_phase phase);
    super.connect_phase(phase);
    `uvm_info("Driver_class", "Inside connect phase!", UVM_HIGH)
    
    
  endfunction
  
  
  task run_phase (uvm_phase phase);
    super.run_phase(phase);
    //drive the logic of the dut which is the fifo using the virtual interface vif
    `uvm_info("Driver_class", "Inside Run phase!", UVM_HIGH)
    
    forever begin
      
      
      item = fifo_sequence_item::type_id::create("item");
      
      //method to get the next item
      
      seq_item_port.get_next_item(item);
      
      //logic to be driven through task drive.
      
      drive(item);
      
      //raise signal of item done
      
      seq_item_port.item_done();
       
      
    end
    
    
  endtask
    
  
  //drive task
  
  task drive (fifo_sequence_item item);
    
    
    @(posedge vif.wclk);
    vif.wrst_n <= item.wrst_n;
    vif.rrst_n <= item.rrst_n;
    vif.w_en <= item.w_en;
    vif.r_en <= item.r_en;
    vif.data_in <= item.data_in;    
    
    
  endtask
  
endclass;
