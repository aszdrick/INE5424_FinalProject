module NiosIISemaphore(CLOCK_50, KEY, LEDG, HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7);
	input CLOCK_50;
	input [3:0] KEY;
	output [8:0] LEDG;
	output [0:6] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7;
	wire   [31:0] to_HEX;

	 NiosII_SoC u0 (
		  .clk_clk(CLOCK_50),     //   clk.clk
		  .reset_reset_n(KEY[0]),  // reset.reset_n
		  .semaphore_conduit_export(to_HEX)  // semaphore_conduit.export
	 );
	 
	assign LEDG[0] = KEY[0];
	hex7seg h0(to_HEX[3:0], HEX0);
	hex7seg h1(to_HEX[7:4], HEX1);
	hex7seg h2(to_HEX[11:8], HEX2);
	hex7seg h3(to_HEX[15:12], HEX3);
	hex7seg h4(to_HEX[19:16], HEX4);
	hex7seg h5(to_HEX[23:20], HEX5);
	hex7seg h6(to_HEX[27:24], HEX6);
	hex7seg h7(to_HEX[31:28], HEX7);
endmodule
