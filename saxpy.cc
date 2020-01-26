#include <iostream>

#include "CL/sycl.hpp"

namespace sycl = cl::sycl;

int main(int argc, char * argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: saxpy.x <vector length>" << std::endl;
        return argc;
    }

    size_t length = std::atoi(argv[1]);
    std::cout << "SAXPY with " << length << " elements" << std::endl;

    std::vector<float> h_X(length,1);
    std::vector<float> h_Y(length,2);
    std::vector<float> h_Z(length,2);

    try {

        sycl::queue q(sycl::default_selector{});

        float A(3);

        auto ctx = q.get_context();

        sycl::buffer<float,1> d_X { h_X.data(), sycl::range<1>(h_X.size()) };
        sycl::buffer<float,1> d_Y { h_Y.data(), sycl::range<1>(h_Y.size()) };
        sycl::buffer<float,1> d_Z { h_Z.data(), sycl::range<1>(h_Z.size()) };

        q.submit([&](sycl::handler& h) {

            auto X = d_X.template get_access<sycl::access::mode::read>(h);
            auto Y = d_Y.template get_access<sycl::access::mode::read>(h);
            auto Z = d_Z.template get_access<sycl::access::mode::read_write>(h);

            h.parallel_for<class nstream>( sycl::range<1>{length}, [=] (sycl::id<1> it) {
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
    catch (std::exception & e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    catch (const char * e) {
        std::cout << e << std::endl;
        return 1;
    }

    size_t errors(0);
    for (size_t i=0; i<length; ++i) {
        //std::cout << h_Z[i] << "\n";
        errors += (h_Z[i] != 7.0f);
    }
    if (errors) {
        std::cerr << "There were " << errors << " errors!" << std::endl;
        return 1;
    }

    return 0;
}
