#include <iostream>

#include <cmath>
#include <cfloat>

#include "CL/sycl.hpp"

namespace sycl = cl::sycl;

const float xval(1);
const float yval(2);
const float zval(2);
const float aval(3);

const float correct = (zval + aval * xval + yval);

int main(int argc, char * argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: saxpy.x <vector length>" << std::endl;
        return argc;
    }

    size_t length = std::atoi(argv[1]);
    std::cout << "SAXPY with " << length << " elements" << std::endl;

    sycl::queue q(sycl::default_selector{});
    auto ctx = q.get_context();
    auto dev = q.get_device();

    float * X = static_cast<float*>(sycl::malloc_shared(length * sizeof(float), dev, ctx));
    float * Y = static_cast<float*>(sycl::malloc_shared(length * sizeof(float), dev, ctx));
    float * Z = static_cast<float*>(sycl::malloc_shared(length * sizeof(float), dev, ctx));

    for (size_t i=0; i<length; i++) {
      X[i] = xval;
      Y[i] = yval;
      Z[i] = zval;
    }

    try {

        const float A(aval);

        q.submit([&](sycl::handler& h) {
            h.parallel_for<class saxpy>( sycl::range<1>{length}, [=] (sycl::id<1> it) {
                const int i = it[0];
                Z[i] += A * X[i] + Y[i];
            });
        });
        q.wait();
    }
    catch (sycl::exception & e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    sycl::free(X, ctx);
    sycl::free(Y, ctx);

    // check for correctness
    size_t errors(0);
    for (size_t i=0; i<length; ++i) {
        if ( std::abs(Z[i] - correct) > FLT_MIN) {
            ++errors;
        }
    }
    if (errors) {
        std::cerr << "There were " << errors << " errors!" << std::endl;
        for (size_t i=0; i<length; ++i) {
            std::cout << i << "," << Z[i] << "\n";
        }
        return 1;
    }

    sycl::free(Z, ctx);

    std::cout << "Program completed without error." << std::endl;

    return 0;
}
