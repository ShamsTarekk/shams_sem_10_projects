class fifo_test extends uvm_test;
  
  `uvm_component_utils(fifo_test)
  fifo_env env;
  fifo_base_sequence reset_seq;
  fifo_test_sequence test_seq;
  
  function new(string name = "fifo_test", uvm_component parent);
    super.new(name, parent);
    `uvm_info("Test_class", "Inside constructor!", UVM_HIGH)
    
  endfunction
  
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    `uvm_info("Test_class", "Inside build phase!", UVM_HIGH)
    env = fifo_env::type_id::create("env", this);
    
  endfunction

  function void connect_phase(uvm_phase phase);
    super.connect_phase(phase);
    `uvm_info("Test_class", "Inside connect phase!", UVM_HIGH)
    
    
  endfunction
  
  
  task run_phase (uvm_phase phase);
    super.run_phase(phase);
    `uvm_info("Test_class", "Inside Run Phase!", UVM_HIGH)
    
    phase.raise_objection(this);
    
    //start our sequences
    //reset_seq

    reset_seq = fifo_base_sequence::type_id::create("reset_seq");
    reset_seq.start(env.agnt.seqr);
    #10;
    
    
    repeat(100) begin
    
    //test_seq
    
    	test_seq = fifo_test_sequence::type_id::create("test_seq");
    	test_seq.start(env.agnt.seqr);
    	#10;
      	assert(test_seq.randomize())
      		else
        		$fatal(0,"ERROR!!!");
      
    end
    
    
    
    
    phase.drop_objection(this);
    
    
  endtask
    
  
endclass;
