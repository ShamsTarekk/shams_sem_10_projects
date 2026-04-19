class fifo_env extends uvm_env;
  
  `uvm_component_utils(fifo_env)
  
  fifo_agent agnt;
  
  function new(string name = "fifo_env", uvm_component parent);
    super.new(name, parent);
    `uvm_info("Env_class", "Inside constructor!", UVM_HIGH)
    
  endfunction
  
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    `uvm_info("Env_class", "Inside build phase!", UVM_HIGH)
    agnt = fifo_agent::type_id::create("agnt", this);
    
  endfunction

  function void connect_phase(uvm_phase phase);
    super.connect_phase(phase);
    `uvm_info("Env_class", "Inside connect phase!", UVM_HIGH)
    
    
  endfunction
  
  
  task run_phase (uvm_phase phase);
    super.run_phase(phase);
    //logic
    
    
  endtask
    
  
endclass;
