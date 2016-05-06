MKOCTFILE=mkoctfile

CXXFLAGS = -std=c++0x 

all: read_spiceraw.oct

read_spiceraw.oct: read_spiceraw.cpp ../convraw.cpp
	$(MKOCTFILE) $(CXXFLAGS) read_spiceraw.cpp

