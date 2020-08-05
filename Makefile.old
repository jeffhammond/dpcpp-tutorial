CXX		 = dpcpp
CXXFLAGS	 = -std=c++17
CXXFLAGS	+= -fsycl
CXXFLAGS	+= -O3
EXTFLAGS	 = -fsycl-unnamed-lambda

all: saxpy.x saxpy-usm.x

saxpy.x: saxpy.cc
	$(CXX) $(CXXFLAGS) $< -o $@

saxpy-usm.x: saxpy-usm.cc
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm -f saxpy.o saxpy.x
	-rm -f saxpy-usm.o saxpy-usm.x

