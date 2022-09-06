#Assume SYSTEMC is set to the SystemC installation
#SystemC installation should have an include directory for the header files
#SystemC installation should have a lib-linux directory for the compiled kernel library


SYSTEM_ARCH=linux64 #Change to the current platform

LIB_DIRS=$(SYSTEMC)/lib-$(SYSTEMC_ARCH)

# Include directories
INCLUDE_DIRS = -I. -I$(SYSTEMC)/include

# Header files used, for dependency checking
HEADERS = fir.h tb.h

# Source files used, for dependency checking
SOURCES = main.cc fir.cc tb.cc

DEPENDENCIES = \
      Makefile \
      $(HEADERS) \
      $(SOURCES)

LIBS= -lsystemc -lstdc++ -lm

TESTS= main

all: $(TESTS)
      ./$(TESTS)
      @make cmp_result

$(TESTS): $(DEPENDENCIES)
      gcc -g -o $@ $(SOURCES) $(INCLUDE_DIRS) -L$(LIB_DIRS) $(LIBS)

clean:
      rm -f $(TESTS) *.dat

# The cmp_result target compares the simulation results with the golden results.
# The testbench writes out the results file (output.dat) in the somulation directory.
# The cmp_result target is automatically run at the end of each simulation.

GOLD_DIR = ./golden
GOLD_FILE = $(GOLD_DIR)/ref_output.dat

cmp_result:
      @echo "**************************************"
      @if diff -w $(GOLD_FILE) ./output.dat; then \
            echo "SIMULATION PASSED"; \
            else \
            echo "SIMULATION FAILED"; \
      fi
      @echo "**************************************"
