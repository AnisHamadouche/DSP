## Instructions
### Prerequisite
 * GNU C Compiler
 * SystemC
 * C++ standard library
 * C++ math library

### Generate Executable
Run the following script to compile the files into an executable program

~~~ gcc -o main main.cc fir.cc tb.cc -I. -I$SYSTEMC/include -L $SYSTEMC/lib-linux64 -lsystemc -lstdc++ -lm ~~~

### Run Simulation
Now run the simulation by typing the following

~~~ ./main ~~~

Simulation data is recorded in the newly generated "output.dat" file.
