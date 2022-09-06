#Compilation

##Prerequisite
 * GNU C Compiler
 * SystemC
 * C++ standard library
 * C++ math library

Run the following script to compile the files into an executable program

gcc -o main main.cc fir.cc tb.cc -I. -I$SYSTEMC/include -L $SYSTEMC/lib-linux64 -lsystemc -lstdc++ -lm

Now run the simulation by typing the following

./main
