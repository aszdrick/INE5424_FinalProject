module NiosIISemaphore(CLOCK_50, KEY, LEDG);
	input CLOCK_50;
	input [3:0] KEY;
	output [8:0] LEDG;

    NiosII_SoC u0 (
        .clk_clk       (CLOCK_50),     //   clk.clk
        .reset_reset_n (KEY[0])  		// reset.reset_n
    );
	 
	 assign LEDG[0] = KEY[0];
endmodule
