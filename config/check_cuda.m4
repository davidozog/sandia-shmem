# CHECK_CUDA([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([CHECK_CUDA], [
  AC_ARG_WITH([cuda],
    [AS_HELP_STRING([--with-cuda=[path]],
       [Location of CUDA library])])

  AS_IF([test "$with_cuda" = "no" -o "$with_cuda" = ""], [happy=no], [happy=yes])

  CUDA_CPPFLAGS=
  CUDA_LDFLAGS=
  CUDA_LIBS="-lcudart"

  saved_CPPFLAGS="$CPPFLAGS"
  saved_LDFLAGS="$LDFLAGS"
  saved_LIBS="$LIBS"
  AS_IF([test ! -z "$with_cuda" -a "$with_cuda" != "yes"],
    [CPPFLAGS="$CPPFLAGS -I$with_cuda/include"
     CUDA_CPPFLAGS="-I$with_cuda/include"
     AS_IF([test -d "$with_cuda/lib64"],
             [LDFLAGS="$LDFLAGS -L$with_cuda/lib64"
              CUDA_LDFLAGS="-L$with_cuda/lib64"],
             [LDFLAGS="$LDFLAGS -L$with_cuda/lib"
              CUDA_LDFLAGS="-L$with_cuda/lib"])])

  AS_IF([test "$happy" = "yes"],
    [AC_CHECK_HEADERS([cuda.h], [], [happy=no])])
  AS_IF([test "$happy" = "yes"],
    [AC_CHECK_LIB([cudart], [cudaMalloc], [], [happy=no])])

  CPPFLAGS="$saved_CPPFLAGS"
  LDFLAGS="$saved_LDFLAGS"
  LIBS="$saved_LIBS"

  AS_IF([test "$happy" = "no"],
        [CUDA_CPPFLAGS=
         CUDA_LDFLAGS=
         CUDA_LIBS=])

  AC_SUBST(CUDA_CPPFLAGS)
  AC_SUBST(CUDA_LDFLAGS)
  AC_SUBST(CUDA_LIBS)

  AS_IF([test "$happy" = "yes"], [$1], [$2])
])
