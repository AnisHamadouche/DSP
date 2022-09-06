#include "tb.h"

void tb::source() {
  //Generation of input sequence
  sc_int<16> tmp;

  //Initialize handshake
  inp_vld.write(0);
  //Reset
  inp.write( 0 );
  rst.write( 1 );
  wait();
  rst.write( 0 );
  wait();

  //Send stimulus to FIR
  for(int i=0; i<64; i++) //A simple pulse input
  {
    if (i>23 && i <29)
      tmp = 256;
    else
      tmp = 0;
    inp_vld.write(1);
    inp.write( tmp );
    //wait for input readiness
    do{
        wait();
    } while( !inp_rdy.read() );
    //Reset
    inp_vld.write(0);
    //Measurment for diagnosis and reporting
    start_time[i] = sc_time_stamp();
  }

  // Hanging simulation guard condition
  wait(10000);
  printf("Hanging simulation stopped by TB source thread. Please check DUT module.\n");
  sc_stop();
}

void tb::sink() {
  sc_int<16> indata;

  //Extract clock period
  sc_clock *clk_p = dynamic_cast<sc_clock *>(clk.get_interface());
  clock_period = clk_p->period();

  //Open simulation output results file
  char output_file[256];
  sprintf( output_file, "./output.dat");
  outfp = fopen(output_file, "wb");
  if (outfp == NULL)
  {
    printf("Couldn't open output.dat for writing");
    exit(0);
  }

  // Initialize port
  outp_rdy.write(0);

  double total_cycles = 0; //Cumulative latencies for all inputs

  //Read output coming from DUT
  for(int i=0; i<64; i++)
  {
    outp_rdy.write(1);
    do {
        wait();
    }while( !outp_vld.read());
    indata = outp.read();
    //Measurment for diagnosis and reporting
    end_time[i] = sc_time_stamp();
    //Calculate and append latency
    total_cycles += (end_time[i] - start_time[i])/ clock_period;
    //Reset
    outp_rdy.write(0);

    //Write to file
    fprintf( outfp, "%d\n", (int)indata);
    cout << i << ":\t" << indata.to_int() << endl;
  }
  // Print latency and throughput
  double total_throughput = (start_time[63] - start_time[0])/clock_period;
  printf("Average latency is %g cycles.\n", (double)(total_cycles/64));
  printf("Average throughput is %g cycles per input.\n", (double)(total_throughput/63));

  // End simulation
  fclose(outfp);
  sc_stop();
}
