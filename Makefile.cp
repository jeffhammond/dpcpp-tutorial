SYCLDIR		 = $(HOME)/CodePlay/latest

CXX		 = $(SYCLDIR)/bin/compute++

CXXFLAGS	+= -O3
CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -ferror-limit=3

CXXFLAGS	+= -sycl-driver
#CXXFLAGS	+= -sycl
CXXFLAGS	+= -sycl-target ptx64
CXXFLAGS	+= -no-serial-memop
CXXFLAGS	+= -I$(SYCLDIR)/include -L$(SYCLDIR)/lib -Wl,-rpath=$(SYCLDIR)/lib -lComputeCpp
CXXFLAGS	+= --gcc-toolchain=/swtools/gcc/7.4.0

all: saxpy.x saxpy-usm.x saxpy-usm2.x saxpy-usm3.x

%.x: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm -f *.o
	-rm -f *.x

