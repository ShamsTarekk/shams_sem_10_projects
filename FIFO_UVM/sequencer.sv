class fifo_sequencer extends uvm_sequencer#(fifo_sequence_item);
  
  `uvm_component_utils(fifo_sequencer)
  
  function new(string name = "fifo_sequencer", uvm_component parent);
    super.new(name, parent);
    `uvm_info("Sequencer_class", "Inside constructor!", UVM_HIGH)
    
  endfunction
  
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    `uvm_info("Sequencer_class", "Inside build phase!", UVM_HIGH)
    
    
  endfunction

  function void connect_phase(uvm_phase phase);
    super.connect_phase(phase);
    `uvm_info("Sequencer_class", "Inside connect phase!", UVM_HIGH)
    
    
  endfunction
  
    
  
endclass;
