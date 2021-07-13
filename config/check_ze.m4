# CHECK_ZE([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([CHECK_ZE], [
  AC_ARG_WITH([ze],
    [AS_HELP_STRING([--with-ze=[path]],
       [Location of ZE library])])

  AS_IF([test "$with_ze" = "no" -o "$with_ze" = ""], [happy=no], [happy=yes])

  ZE_CPPFLAGS=
  ZE_LDFLAGS=
  ZE_LIBS="-lze_loader -ldl -lrt -lpthread"

  saved_CPPFLAGS="$CPPFLAGS"
  saved_LDFLAGS="$LDFLAGS"
  saved_LIBS="$LIBS"
  AS_IF([test ! -z "$with_ze" -a "$with_ze" != "yes"],
    [CPPFLAGS="$CPPFLAGS -I$with_ze/include/"
     ZE_CPPFLAGS="-I$with_ze/include/"
     LDFLAGS="$LDFLAGS -L$with_ze/lib64/"
     ZE_LDFLAGS="-L$with_ze/lib64/"])

  AS_IF([test "$happy" = "yes"],
    [AC_CHECK_HEADERS([level_zero/ze_api.h], [], [happy=no])])
  AS_IF([test "$happy" = "yes"],
    [AC_CHECK_LIB([ze_loader], [zeInit], [], [happy=no], [-ldl -lrt -lpthread])])

  CPPFLAGS="$saved_CPPFLAGS"
  LDFLAGS="$saved_LDFLAGS"
  LIBS="$saved_LIBS"

  AS_IF([test "$happy" = "no"],
        [ZE_CPPFLAGS=
         ZE_LDFLAGS=
         ZE_LIBS=])

  AC_SUBST(ZE_CPPFLAGS)
  AC_SUBST(ZE_LDFLAGS)
  AC_SUBST(ZE_LIBS)

  AS_IF([test "$happy" = "yes"], [$1], [$2])
])
