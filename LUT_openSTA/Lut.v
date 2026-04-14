module Lut (
    input [4:0] a,
    input [4:0] b,
    input clk,
    input reset,
    input [1:0] s,
    input sync,
    output reg [4:0] out
);

reg [4:0] y;

always @(*) begin
    y = 5'b0;
    case (s)
        2'b00: y = a & b;
        2'b01: y = a | b;
        2'b10: y = a + b;
        2'b11: y = a - b;
    endcase
end

always @(posedge clk) begin
    if (reset)
        out <= 5'b0;
    else if (sync)
        out <= y;
end

endmodule