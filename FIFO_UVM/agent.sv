class fifo_agent extends uvm_agent;
  
  `uvm_component_utils(fifo_agent)
  
  fifo_driver drv;
  fifo_monitor mon;
  fifo_sequencer seqr;
  
  function new(string name = "fifo_agent", uvm_component parent);
    super.new(name, parent);
    `uvm_info("Agent_class", "Inside constructor!", UVM_HIGH)
    
  endfunction
  
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    `uvm_info("Agent_class", "Inside build phase!", UVM_HIGH)
    
    drv = fifo_driver::type_id::create("drv", this);
    mon = fifo_monitor::type_id::create("mon", this);
    seqr = fifo_sequencer::type_id::create("seqr", this);
    
    
  endfunction

  function void connect_phase(uvm_phase phase);
    super.connect_phase(phase);
    `uvm_info("Agent_class", "Inside connect phase!", UVM_HIGH)
    
    //connect the driver port to the export of the sequencer to exchange the seq item
    
    drv.seq_item_port.connect(seqr.seq_item_export);
    
    
  endfunction
  
  
  task run_phase (uvm_phase phase);
    super.run_phase(phase);
    //logic
    
    
  endtask
    
  
endclass;
