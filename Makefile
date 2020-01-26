CXX		 = dpcpp
CXXFLAGS	 = -std=c++17
CXXFLAGS	+= -fsycl -fsycl-unnamed-lambda
CXXFLAGS	+= -O3

all: saxpy.x

saxpy.x: saxpy.cc
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm -f saxpy.o saxpy.x

