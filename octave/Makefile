MKOCTFILE=mkoctfile

CXXFLAGS = -std=c++0x 

FILES = read_spiceraw.oct

all: $(FILES)

$(FILES): read_spiceraw.cpp ../convraw.cpp
	$(MKOCTFILE) $(CXXFLAGS) read_spiceraw.cpp

clean:
	rm $(FILES)

