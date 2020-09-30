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
        std::cerr << "Usage: saxpy-usm.x <vector length>" << std::endl;
        return argc;
    }

    size_t length = std::atoi(argv[1]);
    std::cout << "SAXPY USM with " << length << " elements" << std::endl;

    sycl::queue q(sycl::default_selector{});

    auto X = sycl::malloc_shared<float>(length, q);
    auto Y = sycl::malloc_shared<float>(length, q);
    auto Z = sycl::malloc_shared<float>(length, q);

    for (size_t i=0; i<length; i++) {
      X[i] = xval;
      Y[i] = yval;
      Z[i] = zval;
    }

    try {

        const float A(aval);

        q.submit([&](sycl::handler& h) {
            h.parallel_for<class saxpy>( sycl::range<1>{length}, [=] (sycl::id<1> i) {
                Z[i] += A * X[i] + Y[i];
            });
        });
        q.wait();
    }
    catch (sycl::exception & e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

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

    sycl::free(X, q);
    sycl::free(Y, q);
    sycl::free(Z, q);

    std::cout << "Program completed without error." << std::endl;

    return 0;
}
