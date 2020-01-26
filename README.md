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

