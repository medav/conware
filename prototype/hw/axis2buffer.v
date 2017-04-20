module axis2buffer #(
    parameter DWIDTH = 32,
    parameter WIDTH = 4,
    parameter HEIGHT = 4
)( 
    // Control signals
    clk,
    rstn,

    // Color conversion signals
    alive_color,
    dead_color,
    
    // AXIS Connection
    S_AXIS_TDATA,
    S_AXIS_TVALID,
    S_AXIS_TREADY,
    S_AXIS_TLAST,
    
    // Output to conware computation
    out_data,
    out_valid,
    out_ready
);

    // Port descriptions
    input clk;
    input rstn;

    input [DWIDTH-1:0] alive_color;
    input [DWIDTH-1:0] dead_color;

    input [DWIDTH-1:0] S_AXIS_TDATA;
    input S_AXIS_TVALID;
    input S_AXIS_TLAST;
    output S_AXIS_TREADY;

    output [WIDTH*HEIGHT-1:0] out_data;
    output out_valid;
    input out_ready;

    // State params
    reg state;
    localparam Wait = 0;
    localparam Read = 1;

    // Internal values
    reg [DWIDTH - 1:0] buffer [WIDTH*HEIGHT-1:0];
    reg [31:0] counter;

    assign S_AXIS_TREADY = (state == Read);

    // Read state machine
    always @(posedge clk) begin
        if (!rstn) begin
            counter <= 32'h00000000;
            state <= Wait;
        end else begin
            case (state)
            Wait: begin
                if (out_ready) begin
                    state <= Read;
                end else begin
                    state <= Wait;

                end
            end
            Read: begin
                
                if (S_AXIS_TVALID == 1) begin
                    buffer[counter] <= S_AXIS_TDATA;
                    if (counter == WIDTH*HEIGHT-1) begin
                        counter <= 0;
                        state <= Wait;
                    end else begin
                        counter <= counter + 1;
                    end

                end else begin
                    counter <= counter;
                end

            end
            endcase
        end
    end

    // Color conversion to binary values because Veriloc can't pass 2D arrays as I/O
    genvar i;
    generate 
        for (i = 0; i < WIDTH*HEIGHT; i=i+1) begin : converter_block
            assign out_data[i] = (buffer[i] == alive_color)? 1'b1 : 1'b0;
        end
    endgenerate
endmodule