
# Adjust these lines to match your setup
PERFSDK=/Users/thead/Downloads/IntelPerformanceCounterMonitorV2.6/
CC=/usr/bin/clang++ -std=c++11 -stdlib=libc++
OPT=-O3

UNAME:=$(shell uname)
ifeq ($(UNAME), Linux)
LIB= -lpthread -lrt
endif
ifeq ($(UNAME), Darwin)
LIB= -lpthread -lPcmMsr
endif

all: copy example search

copy:
	cp -a $(PERFSDK)/msr.h $(PERFSDK)/pci.h $(PERFSDK)/cpucounters.h \
	 $(PERFSDK)/client_bw.h $(PERFSDK)/types.h $(PERFSDK)/width_extender.h \
	 $(PERFSDK)/utils.h .
	cp -a $(PERFSDK)/msr.cpp $(PERFSDK)/pci.cpp $(PERFSDK)/cpucounters.cpp \
	 $(PERFSDK)/client_bw.cpp .

msr.o: msr.h msr.cpp
	$(CC) $(OPT) -c msr.cpp

pci.o: pci.h pci.cpp
	$(CC) $(OPT) -c pci.cpp

client_bw.o: client_bw.h pci.h client_bw.cpp
	$(CC) $(OPT) -c client_bw.cpp

cpucounters.o: msr.h msr.cpp cpucounters.h cpucounters.cpp types.h width_extender.h
	$(CC) $(OPT) -c cpucounters.cpp

example.o: example.cpp
	$(CC) $(OPT) -c example.cpp

%.o: %.cpp
	$(CC) $(OPT) -c $<

example: example.o msr.o pci.o cpucounters.o client_bw.o
	$(CC) $(LIB) -o example.x $^

search: search.o msr.o pci.o cpucounters.o client_bw.o
	$(CC) $(LIB) -o search.x $^

vectors: vectors.o msr.o pci.o cpucounters.o client_bw.o
	$(CC) $(LIB) -o vectors.x $^

clean:
	rm -rf *.x *.o *~
