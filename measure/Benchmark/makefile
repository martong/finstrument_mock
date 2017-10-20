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

# Configure these for your need
MSAN_BASE = /Users/mg/Work/finstrument_mock/
CXX = $(MSAN_BASE)/compiler/build.release/bin/clang++
MSAN = $(MSAN_BASE)/rt/finstrument_mock/build.release/compiler-rt
MSAN_I = $(MSAN_BASE)/rt/finstrument_mock/compiler-rt/include

LDFLAGS = -rpath $(MSAN) -L $(MSAN) -lmock_san
INCLUDE = -I. -I$(MSAN_I)
CPPFLAGS += $(INCLUDE) -g -std=c++11

CLIBS = -lm
CPPLIBS = -lm

DEPENDENCYFLAG = -M


#
# our target programs
#
BINARIES = machine \
stepanov_abstraction \
stepanov_vector \
functionobjects

#
# Build rules
#
all : $(BINARIES)

machine: machine.o trace.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) machine.o trace.o -o machine
stepanov_vector: stepanov_vector.o trace.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) stepanov_vector.o trace.o -o stepanov_vector
stepanov_abstraction: stepanov_abstraction.o trace.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) stepanov_abstraction.o trace.o -o stepanov_abstraction
functionobjects: functionobjects.o trace.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) functionobjects.o trace.o -o functionobjects


SUFFIXES:
.SUFFIXES: .c .cpp


# declare some targets to be fakes without real dependencies
.PHONY : clean dependencies

REPORT_FILE = report.txt

# remove all the stuff we generate
clean :
		rm -f *.o $(BINARIES) $(REPORT_FILE)


# generate dependency listing from all the source files
# used for double checking problems with headers
# this does NOT go in the makefile
SOURCES = $(wildcard *.c)  $(wildcard *.cpp)
dependencies :   $(SOURCES)
	$(CXX) $(DEPENDENCYFLAG) $(CPPFLAGS) $^




#
# Run the benchmarks and generate a report
#
report:  $(BINARIES)
	echo "##STARTING Version 1.0" > $(REPORT_FILE)
	date >> $(REPORT_FILE)
	echo "##CPPFlags: $(CPPFLAGS)" >> $(REPORT_FILE)
	./machine >> $(REPORT_FILE)
	./stepanov_abstraction 20000 >> $(REPORT_FILE)
	./stepanov_vector 15000 >> $(REPORT_FILE)
	./functionobjects 200 >> $(REPORT_FILE)
	date >> $(REPORT_FILE)
	echo "##END Version 1.0" >> $(REPORT_FILE)
