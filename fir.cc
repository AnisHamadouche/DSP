//FIR MAIN THREAD
#include "fir.h"

//Constants declarion goes here
const sc_uint<8> coef[5] = { //AxC candidate
  18,
  77,
  107,
  77,
  18
};
//FIR main thread
void fir::fir_main(void)
{
  //Variables declaration goes here
  sc_int<16> taps[5]; //AxC candidate
  //Reset protocol
  //Reset internal variables
  for(int i=0; i<5; i++) {
    taps[i]=0;
  }
  //Initialize handshake
  inp_rdy.write(0);
  outp_vld.write(0);
  //Reset outputs
  outp.write( 0);
  wait();

  //Application: FIR
  while( true ) {
    sc_int<16> in_val;
    sc_int<16> out_val;

    inp_rdy.write(1);
    do{
        wait();
    }while( !inp_vld.read());
    in_val = inp.read();
    inp_rdy.write(0);

    //Read input into shift register
    for (int i = 5-1; i>0; i--){
      taps[i] = taps[i-1]; //sc_int<16> taps[5];
    }
    taps[0] = in_val;

    //Algorithm code: multiply and accumulate
    for (int i=0; i<5; i++){
      out_val += coef[i]*taps[i]; //sc_int<16> val //AxC candidate
    }
    //write outputs
    outp_vld.write(1);
    outp.write( out_val );
    do{
      wait();
    } while( !outp_rdy.read());
    outp_vld.write(0);
  }//While
}
