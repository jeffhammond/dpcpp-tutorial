# Intel Data Parallel C++ Tutorial

# What is DPC++?

This is an introduction to Intel's Data Parallel C++ programming model, or DPC++ for short.
DPC++ is based on Khronos SYCL, which means it is a modern C++ parallel programming model.

## What is the difference between SYCL and DPC++?

SYCL 1.2.1 is the latest Khronos standard for SYCL.
Intel and other members of the SYCL working group are in the process of defining the next version of the language specification.
DPC++ contains a number of language extensions that make SYCL easier to use.
Intel is proposing many of its language extensions to Khronos for standardization.
Implementing these extensions in the DPC++ compiler help the community evaluate their effectiveness.

## External resources

* [Khronos SYCL 1.2.1 specification](https://www.khronos.org/registry/SYCL/specs/sycl-1.2.1.pdf)
* [oneAPI DPC++ documentation](https://spec.oneapi.com/oneAPI/Elements/dpcpp/dpcpp_root.html)
* [SYCL/DPC++ compiler on GitHub](https://github.com/intel/llvm/blob/sycl/sycl/doc/GetStartedWithSYCLCompiler.md)
* [DPC++ language extensions](https://github.com/intel/llvm/tree/sycl/sycl/doc/extensions)

# Who is this tutorial for?

This tutorial is for programmers who have a decent understanding of C++ and parallelism already.
Teaching C++ and parallelism is hard and there is a lot of material out there already.
There is far less information on SYCL itself, and even less about DPC++, so that is our focus.

## Who is this tutorial *not* for?

When I tell people about SYCL, I often say "if you like modern C++, you'll like SYCL, because it's definitely modern C++."
A corollary to this is, if you hate C++, you'll hate SYCL and DPC++.
So, if you don't want to write modern C++, this tutorial is not for you.

OpenMP 5.0 offers many of the same features as SYCL/DPC++ but supports the ISO language triumvirate of C++, C and Fortran.
If you want to program CPUs and GPUs using Fortran, C or pre-modern C++ (i.e. before C++11) using an open industry standard, try OpenMP.

Another alternative to SYCL/DPC++ without the C++ is OpenCL.
OpenCL is a lot more verbose than SYCL, but if you are a C programmer, you probably like suffering anyways 🤭

# Background information

One of the important C++ concepts used in SYCL is a lambda;
if you don't know what a lambda is, please see [this introduction](https://raja.readthedocs.io/en/master/tutorial.html#a-little-c-lambda-background).

SYCL is derived from OpenCL and the execution models are the same.
If you need help understanding the SYCL/OpenCL execution model, please checkout [this overview](https://www.khronos.org/assets/uploads/developers/library/2012-pan-pacific-road-show-June/OpenCL-Details-Taiwan_June-2012.pdf).

# The tutorial

We will start with vector addition, which is the "hello, world" of HPC and numerical computation.
Printing "Hello, world!" does not make a lot of sense in a programming model used for doing lots of things in parallel.

## Vector addition in SYCL

The operation we are trying to implement is SAXPY, which stands for "**S**ingle-precision **A** times **X** **P**lus **Y**", where
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
h.parallel_for<class saxpy>( sycl::range<1>{length}, [=] (sycl::id<1> it) {
    const int i = it[0];
    Z[i] += A * X[i] + Y[i];
});
```

As you might have guessed, `parallel_for` is a parallel for loop.
The loop body is expressed as a lambda, which is the part inside of the `{}`.

The loop iterator is expressed in terms of a `sycl::range` and an `sycl::id`.
In our simple example, both are one-dimension, as indicated by the `<1>`.
SYCL ranges and ids can be one-, two- or three-dimensional (OpenCL and CUDA have the same limitation).

Why do we cast the loop index `it` to an integer `i`?
We'll get to that when we go into detail on `sycl::range`s.

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

## Structure of SYCL program

One of the challenges of heterogeneous programming is that there are multiple types of processing elements and often different memory types.  These things make compilers and runtimes more complicated.

The SYCL programming model embraces heterogeneous execution, although at a much higher level than OpenCL.  Not everything is explicit either - unlike other popular GPU programming models, SYCL kernels can be inlined into the host program flow, which improves readability.

Whenever we want to compute on a device, we need to create a work queue:
```c++
sycl::queue q(sycl::default_selector{});
```
The default selector favors a GPU if present and a CPU otherwise.
One can create queues associated with specific device types using the follow:
```c++
sycl::queue q(sycl::host_selector{});        // run on the CPU without a runtime (e.g. no OpenCL)
sycl::queue q(sycl::cpu_selector{});         // run on the CPU with a runtime (e.g. OpenCL)
sycl::queue q(sycl::gpu_selector{});         // run on the GPU
sycl::queue q(sycl::accelerator_selector{}); // run on an FPGA or other acclerator
```
The host and CPU selectors may lead to significantly different results, even though they target the same hardware, because the host selector might use a sequential implementation optimized for debugging, while the CPU selector uses the OpenCL runtime and runs across all the cores.  Furthermore, the OpenCL JiT compiler might generate different code because it's using a different compiler altogether.  Don't assume that just because the host is a CPU, that host and CPU mean the same thing in SYCL!

The full program is included in the repo: [saxpy.cc](saxpy.cc).
