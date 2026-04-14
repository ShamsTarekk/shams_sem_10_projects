`timescale 1ns/1ps

module Lut_tb;

reg [4:0] a;
reg [4:0] b;
reg clk;
reg reset;
reg [1:0] s;
reg sync;
wire [4:0] out;

// DUT
Lut uut (
    .a(a),
    .b(b),
    .clk(clk),
    .reset(reset),
    .s(s),
    .sync(sync),
    .out(out)
);

// Clock (10ns period)
always #5 clk = ~clk;

// waveform
initial begin
    $dumpfile("wave.vcd");
    $dumpvars(0, Lut_tb);
end

// stimulus
initial begin
    clk = 0;
    reset = 1;
    sync = 0;
    a = 0;
    b = 0;
    s = 0;

    // ---- Reset phase ----
    repeat (2) @(posedge clk);
    reset <= 0;

    // ---- AND ----
    @(negedge clk);
    a <= 5'b10101;
    b <= 5'b11000;
    s <= 2'b00;
    sync <= 1;

    @(posedge clk);

    // ---- OR ----
    @(negedge clk);
    s <= 2'b01;

    @(posedge clk);

    // ---- ADD ----
    @(negedge clk);
    a <= 5'd10;
    b <= 5'd5;
    s <= 2'b10;

    @(posedge clk);

    // ---- SUB ----
    @(negedge clk);
    a <= 5'd15;
    b <= 5'd3;
    s <= 2'b11;

    @(posedge clk);

    // ---- async mode off ----
    @(negedge clk);
    sync <= 0;
    a <= 5'b11111;
    b <= 5'b00001;
    s <= 2'b00;

    @(posedge clk);

    $finish;
end

// monitor
initial begin
    $monitor("Time=%0t | a=%b b=%b s=%b sync=%b out=%b",
             $time, a, b, s, sync, out);
end

endmodule