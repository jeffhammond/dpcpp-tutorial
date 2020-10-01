CXX		 = dpcpp
CXXFLAGS	 = -std=c++17
CXXFLAGS	+= -fsycl
CXXFLAGS	+= -O3
EXTFLAGS	 = -fsycl-unnamed-lambda

all: saxpy.x saxpy-usm.x saxpy-usm2.x saxpy-usm3.x

%.x: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm -f *.o
	-rm -f *.x

