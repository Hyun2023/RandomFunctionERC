
IRRAM = $(HOME)/iRRAM/installed
RND   = $(HOME)/iRRAM-Random

CXXFLAGS = -Wall -O2

override CPPFLAGS += -I$(IRRAM)/include -I$(RND)/include
override LDFLAGS  += -L$(RND) -L$(IRRAM)/lib -Wl,-rpath,$(IRRAM)/lib
override LDLIBS   += -lrandom -liRRAM -lmpfr -lgmp
