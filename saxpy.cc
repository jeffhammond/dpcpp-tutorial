#include "tutorial.hpp"

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

    // host data
    std::vector<float> h_X(length,xval);
    std::vector<float> h_Y(length,yval);
    std::vector<float> h_Z(length,zval);

    sycl::queue q(sycl::default_selector{});

    try {

        const float A(aval);

        sycl::buffer<float,1> d_X { h_X.data(), sycl::range<1>(h_X.size()) };
        sycl::buffer<float,1> d_Y { h_Y.data(), sycl::range<1>(h_Y.size()) };
        sycl::buffer<float,1> d_Z { h_Z.data(), sycl::range<1>(h_Z.size()) };

        q.submit([&](sycl::handler& h) {

#if 1
            auto X = d_X.get_access<sycl::access::mode::read>(h);
            auto Y = d_Y.get_access<sycl::access::mode::read>(h);
            auto Z = d_Z.get_access<sycl::access::mode::read_write>(h);
#else
            sycl::accessor X(d_X,h,sycl::read_only);
            sycl::accessor Y(d_Y,h,sycl::read_only);
            sycl::accessor Z(d_Z,h,sycl::read_write);
#endif

            h.parallel_for<class axpy>( sycl::range<1>{length}, [=] (sycl::id<1> it) {
                const size_t i = it[0];
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
        if ( std::abs(h_Z[i] - correct) > FLT_MIN) {
            ++errors;
        }
    }
    if (errors) {
        std::cerr << "There were " << errors << " errors!" << std::endl;
        for (size_t i=0; i<length; ++i) {
            std::cout << i << "," << h_Z[i] << "\n";
        }
        return 1;
    }
    std::cout << "Program completed without error." << std::endl;

    return 0;
}
