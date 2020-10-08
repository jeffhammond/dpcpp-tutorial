#include "tutorial.hpp"

const float xval(1);
const float yval(2);
const float zval(2);
const float aval(3);

const float correct = (zval + aval * xval + yval);

int main(int argc, char * argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: saxpy-usm2.x <vector length>" << std::endl;
        return argc;
    }

    size_t length = std::atoi(argv[1]);
    std::cout << "SAXPY USMv2 with " << length << " elements" << std::endl;

    // host data
    std::vector<float> h_X(length,xval);
    std::vector<float> h_Y(length,yval);
    std::vector<float> h_Z(length,zval);

    sycl::queue q(sycl::default_selector{});

    auto X = syclx::malloc_device<float>(length, q);
    auto Y = syclx::malloc_device<float>(length, q);
    auto Z = syclx::malloc_device<float>(length, q);

    const size_t bytes = length * sizeof(float);

    q.memcpy(X, h_X.data(), bytes);
    q.memcpy(Y, h_Y.data(), bytes);
    q.memcpy(Z, h_Z.data(), bytes);
    q.wait();

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

    q.memcpy(h_Z.data(), Z, bytes);
    q.wait();

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

    syclx::free(X, q);
    syclx::free(Y, q);
    syclx::free(Z, q);

    std::cout << "Program completed without error." << std::endl;

    return 0;
}
