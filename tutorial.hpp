#include <cmath>
#include <cfloat>

#include <iostream>

#include "CL/sycl.hpp"
namespace sycl = cl::sycl;

#ifdef __COMPUTECPP__
#include "SYCL/experimental.hpp"
namespace syclx = cl::sycl::experimental;
#else
namespace syclx = cl::sycl;
#endif
