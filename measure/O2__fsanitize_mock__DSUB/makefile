#
#   Copyright 2007-2008 Adobe Systems Incorporated
#   Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
#   or a copy at http://stlab.adobe.com/licenses.html )
#

###################################################
#
# Makefile for C++ benchmarks
#
###################################################



#
# Macros
#



# GCC
#CC = /usr/bin/gcc
#CXX = /usr/bin/g++


# GCC 4.2
#CC = /usr/bin/gcc-4.2
#CXX = /usr/bin/g++-4.2


# LLVM-gcc old
#CC = /usr/local/bin/llvm-gcc
#CXX = /usr/local/bin/llvm-g++


# LLVM-gcc-4.2
#CC = /usr/bin/llvm-gcc-4.2
#CXX = /usr/bin/llvm-g++-4.2


# Intel compiler
# NOTE - incorrectly installed on MacOS, links to 64 bit compiler by mistake, so needs to be forced to 32 bit default
#CC = icc -m32
#CXX = icpc -m32


CXX = /Users/mg/Work/finstrument_mock/compiler/build.release/bin/clang++
MSAN = /Users/mg/Work/finstrument_mock/rt/finstrument_mock/build.release/compiler-rt
MSAN_I = /Users/mg/Work/finstrument_mock/rt/finstrument_mock/compiler-rt/include
LDFLAGS = -rpath $(MSAN) -L $(MSAN) -lmock_san

INCLUDE = -I. -I$(MSAN_I)

CFLAGS = 
CPPFLAGS = $(INCLUDE) -O2 -fsanitize=mock

CLIBS = -lm
CPPLIBS = -lm

DEPENDENCYFLAG = -M


#
# our target programs
#

BINARIES = machine \
stepanov_abstraction \
stepanov_vector \
loop_unroll \
simple_types_loop_invariant \
functionobjects \
simple_types_constant_folding


#
# Build rules
#

all : $(BINARIES)


SUFFIXES:
.SUFFIXES: .c .cpp


# declare some targets to be fakes without real dependencies

.PHONY : clean dependencies

# remove all the stuff we build

clean : 
		rm -f *.o $(BINARIES)


# generate dependency listing from all the source files
# used for double checking problems with headers
# this does NOT go in the makefile

SOURCES = $(wildcard *.c)  $(wildcard *.cpp)
dependencies :   $(SOURCES)
	$(CXX) $(DEPENDENCYFLAG) $(CPPFLAGS) $^




#
# Run the benchmarks and generate a report
#
REPORT_FILE = report.txt

report:  $(BINARIES)
	echo "##STARTING Version 1.0" > $(REPORT_FILE)
	date >> $(REPORT_FILE)
	echo "##CFlags: $(CFLAGS)" >> $(REPORT_FILE)
	echo "##CPPFlags: $(CPPFLAGS)" >> $(REPORT_FILE)
	./machine >> $(REPORT_FILE)
	./stepanov_abstraction >> $(REPORT_FILE)
	./stepanov_vector >> $(REPORT_FILE)
	./functionobjects >> $(REPORT_FILE)
	#./simple_types_constant_folding >> $(REPORT_FILE)
	#./simple_types_loop_invariant >> $(REPORT_FILE)
	#./loop_unroll >> $(REPORT_FILE)
	date >> $(REPORT_FILE)
	echo "##END Version 1.0" >> $(REPORT_FILE)

