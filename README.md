# Intel Data Parallel C++ Tutorial

# What is DPC++?

This is an introduction to the Data Parallel C++ programming model, or DPC++ for short.
DPC++ is based on Khronos SYCL, which means it is a modern C++ parallel programming model.
SYCL 1.2.1 is the latest Khronos standard,
although the SYCL 2020 provisional specific is available for review.
Intel and other members of the SYCL working group are
in the process of finalizing the next version of the language specification.
DPC++ contains a number of extensions that make SYCL easier to use,
although many of these extensions are expected to be part of SYCL 2020.
Implementing extensions in the DPC++ compiler helps the community
evaluate their effectiveness before standardization.

# Who is this tutorial for?

This tutorial is for programmers who have a decent understanding of C++ and parallelism already.
Teaching C++ and parallelism is hard and there is a lot of material out there already.
There is far less information on SYCL itself, and even less about DPC++, so that is our focus.

One of the important C++ concepts used in SYCL is a lambda;
if you don't know what a lambda is, please see [this introduction](https://raja.readthedocs.io/en/master/tutorial.html#a-little-c-lambda-background).

SYCL is derived from OpenCL and the execution models are quite similar.
If you need help understanding the SYCL/OpenCL execution model, please checkout [this overview](https://www.khronos.org/assets/uploads/developers/library/2012-pan-pacific-road-show-June/OpenCL-Details-Taiwan_June-2012.pdf).

# Who is this tutorial *not* for?

When I tell people about SYCL, I often say "if you like modern C++, you'll like SYCL, because it's definitely modern C++."
A corollary to this is, if you hate C++, you'll hate SYCL and DPC++.
So, if you don't want to write modern C++, this tutorial is not for you.

OpenMP 5.0 offers many of the same features as SYCL/DPC++ but supports the ISO language triumvirate of C++, C and Fortran.
If you want to program CPUs and GPUs using Fortran, C or pre-modern C++ (i.e. before C++11) using an open industry standard, try OpenMP.

Another alternative to SYCL/DPC++ without the C++ is OpenCL.
OpenCL is a lot more verbose than SYCL, but if you are a C programmer, you probably like suffering anyways 🤭

# The tutorial

We will start with vector addition, which is the "hello, world" of HPC and numerical computation.
Printing "Hello, world!" does not make a lot of sense in a programming model used for doing lots of things in parallel.

## Vector addition in SYCL

The operation we are trying to implement is SAXPY, which stands for "**S**ingle-precision **A** times **X** **P**lus **Y**", which can be implemented in C or C++ as follows:
```c++
for (size_t i=0; i<length; ++i) {
    Z[i] += A * X[i] + Y[i];
}
```
There are a lot of ways to write this in C++.
For example, we could use ranges, which would make the code look a bit more like the upcoming SYCL version, but teaching you every possible way to write a loop in C++ isn't the point of this tutorial, and everybody understands the version that looks like C.

Here is the same loop in SYCL.
There is a lot to unpack here, so we'll break down in pieces.
```c++
h.parallel_for<class saxpy>( sycl::range<1>{length}, [=] (sycl::id<1> i) {
    Z[i] += A * X[i] + Y[i];
});
```

As you might have guessed, `parallel_for` is a parallel for loop.
The loop body is expressed as a lambda -- the lambda is the code that looks like [..]{..}.

The loop iterator is expressed in terms of a `sycl::range` and an `sycl::id`.
In our simple example, both are one-dimension, as indicated by the `<1>`.
SYCL ranges and ids can be one-, two- or three-dimensional (OpenCL and CUDA have the same limitation).

It may be a bit unfamiliar to write loops like this, but it is consistent with how lambdas work.
However, if you have ever have used 
[parallel STL](https://github.com/intel/parallelstl),
[TBB](https://github.com/intel/tbb),
[Kokkos](https://github.com/kokkos/kokkos/), or
[RAJA](https://github.com/LLNL/RAJA),
you will recognize the pattern.

You might wonder about the `<class saxpy>` template argument to `parallel_for`.
This is just a way to name the kernel, which is necessary because you might want to use SYCL with a different host C++ compiler than the SYCL device compiler, in which case, the two compilers need a way to agree on the kernel name.
In many SYCL compilers, such as Intel DPC++, this isn't necessary and one can tell the compiler to not worry about looking for names by using the option `-fsycl-unnamed-lambda`.

We won't try to explain what the `h` in `h.parallel_for` is right now.
That will be covered later.

## SYCL queues

One of the challenges of heterogeneous programming is that there are multiple types of processing elements and often different memory types.  These things make compilers and runtimes more complicated.

The SYCL programming model embraces heterogeneous execution, although at a much higher level than OpenCL.  Not everything is explicit either - unlike other popular GPU programming models, SYCL kernels can be inlined into the host program flow, which improves readability.

Whenever we want to compute on a device, we need to create a work queue:
```c++
sycl::queue q(sycl::default_selector{});
```
The default selector favors a GPU if present and a CPU otherwise.
One can create queues associated with specific device types using the follow:
```c++
sycl::queue q(sycl::host_selector{});        // run on the CPU without a runtime (i.e., no OpenCL)
sycl::queue q(sycl::cpu_selector{});         // run on the CPU with a runtime (e.g., OpenCL)
sycl::queue q(sycl::gpu_selector{});         // run on the GPU
sycl::queue q(sycl::accelerator_selector{}); // run on an FPGA or other acclerator
```
The host and CPU selectors may lead to significantly different results, even though they target the same hardware, because the host selector might use a sequential implementation optimized for debugging, while the CPU selector uses the OpenCL runtime and runs across all the cores.  Furthermore, the OpenCL JiT compiler might generate different code because it's using a different compiler altogether.  Don't assume that just because the host is a CPU, that host and CPU mean the same thing in SYCL!

## Managing data in SYCL using `buffers`

The canonical way to manage data in SYCL is with buffers.
A SYCL buffer is an opaque container.
This is an elegant design, but some applications would like pointers, which are provided by the USM extension discussed later.
```c++
// T is a data type, e.g. float
std::vector<T> h_X(length,xval);
sycl::buffer<T,1> d_X { h_X.data(), sycl::range<1>(h_X.size()) };
```
In the previous example, the user allocates a C++ container on the host and then hands it over to SYCL.
Until the destructor of the SYCL buffer is invoked, the user cannot access the data through a non-SYCL mechanism.

SYCL accessors are the important aspect of SYCL data management with buffers, which are explained below.

## Controlling device execution

Because device code may require a different compiler or code generation mechanism from the host, it is necessary to clearly identify sections of device code.
Below we see how this looks in SYCL 1.2.1.
We use the `submit` method to enqueue work to the device queue, `q`.
This method returns an opaque handler, against which we execute kernels, in this case via `parallel_for`.
```
q.submit([&](sycl::handler& h) {
    ...
    h.parallel_for<class nstream>( sycl::range<1>{length}, [=] (sycl::id<1> i) {
        ...
    });
});
q.wait();
```
We can synchronize device execution using the `wait()` method.
There are finer grain methods for synchronizing device execution, but we start with simplest one, which is a heavy hammer.

Some users may find the above a bit verbose, particularly compared to models like Kokkos.
The Intel DPC++ compiler supports a terse syntax, which will be covered below.

## Compute kernels and buffers

SYCL accessors are the final piece of the puzzle in our first SYCL program.
Accessors may be unfamiliar to GPU programmers, but have a number of nice properties relative to other methods.
While SYCL allows the programmer to move data explicitly using e.g. the `copy()` method,
the accessor methods do not require this, because they generate a dataflow graph that the compiler and runtime
can use to move data at the right time.
This is particularly effective when multiple kernels are invoked in sequence;
in this case, the SYCL implementation will deduce that data is reused and not copy it back to the host unnecessarily.
Furthermore, data movement can be scheduled asynchronously, i.e. overlapped with device execution.
While expert GPU programmers can do this manually, we often find that SYCL accessors lead to better
performance than OpenCL programs where programmers must move data explicitly.
```c++
q.submit([&](sycl::handler& h) {

    auto X = d_X.template get_access<sycl::access::mode::read>(h);
    auto Y = d_Y.template get_access<sycl::access::mode::read>(h);
    auto Z = d_Z.template get_access<sycl::access::mode::read_write>(h);

    h.parallel_for<class nstream>( sycl::range<1>{length}, [=] (sycl::id<1> i) {
        ...
    });
});
```
Because programming models that assume pointers are handles to memory have a hard time with SYCL accessors,
the USM extension makes accessors unnecessary.
This places a higher burden on the programmer, but is necessary for compatibility with legacy code.

## Review of our first SYCL program

Below we include all of the components of our SYCL saxpy program that we just described.
The full program is included in the repo: [saxpy.cc](saxpy.cc).
```c++
    std::vector<float> h_X(length,xval);
    std::vector<float> h_Y(length,yval);
    std::vector<float> h_Z(length,zval);

    try {

        sycl::queue q(sycl::default_selector{});

        const float A(aval);

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
        std::abort();
    }
```

# External resources

* [Khronos SYCL 1.2.1 specification](https://www.khronos.org/registry/SYCL/specs/sycl-1.2.1.pdf)
* [oneAPI DPC++ documentation](https://spec.oneapi.com/oneAPI/Elements/dpcpp/dpcpp_root.html)
* [SYCL/DPC++ compiler on GitHub](https://github.com/intel/llvm/blob/sycl/sycl/doc/GetStartedWithSYCLCompiler.md)
* [DPC++ language extensions](https://github.com/intel/llvm/tree/sycl/sycl/doc/extensions)
