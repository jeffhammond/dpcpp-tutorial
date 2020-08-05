AC_PREREQ(2.62)

AC_INIT([saxpy],[0.1])
AC_CONFIG_AUX_DIR(m4)
AC_CONFIG_MACRO_DIR(m4)
AM_INIT_AUTOMAKE([-Wall -Werror foreign 1.11 color-tests parallel-tests subdir-objects])

AC_PROG_CXX([dpcpp compute++ syclcc-clang clang++])
AC_PROG_CXX_C_O

# SYCL 2020 is based on C++17
AX_CXX_COMPILE_STDCXX([17])

AC_LANG([C++])

AC_CHECK_HEADER([CL/sycl.hpp], AC_MSG_ERROR([CL/sycl.hpp not found.]))

AC_CHECK_HEADER(mpi.h,,
AC_MSG_ERROR([mpi.h is required])
)

AC_MSG_CHECKING([SYCL buffer copy])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM([#include <CL/sycl.hpp>],
                                    [[char dummy[PATH_MAX];]])],
                   [
                      AC_MSG_RESULT([yes])
                      AC_DEFINE_UNQUOTED([HAVE_PATH_MAX], 1, [Define to 1 if you have the PATH_MAX macro.])
                    ],
                    [
                      AC_MSG_RESULT([no])
                      AC_DEFINE_UNQUOTED([HAVE_PATH_MAX], 0, [Define to 1 if you have the PATH_MAX macro.])
                    ]
                 )



