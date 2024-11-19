# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([your_project], [1.0], [your_email@example.com])
AM_INIT_AUTOMAKE([gnu])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Check for programs
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_YACC

# Check for glibc
AC_CHECK_HEADERS([gnu/libc-version.h])

# Check for C11 support
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#error "C11 support required"
#endif
]])], [
  AC_DEFINE([HAVE_C11], [1], [Define to 1 if the compiler supports C11])
], [
  # Check for C99 support
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#error "C99 support required"
#endif
]])], [
    AC_DEFINE([HAVE_C99], [1], [Define to 1 if the compiler supports C99])
  ], [
    # Check for C89 support
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199409L
#error "C89 support required"
#endif
]])], [
      AC_DEFINE([HAVE_C89], [1], [Define to 1 if the compiler supports C89])
    ], [
      AC_MSG_ERROR([No acceptable C compiler found!])
    ])
  ])
])

# Checks for libraries.
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h string.h stdarg.h ctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

# Define the output files
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tests/Makefile])
AC_OUTPUT
