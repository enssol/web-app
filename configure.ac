// Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
// SPDX-License-Identifier: AGPL-3.0-or-later

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Autoconf configuration file for the web-app project.
# Information about the project is stored in the `configure.ac` file.
AC_PREREQ([2.72])
AC_INIT(web-app, 1.0, https://github.com/enssol/web-app/issues/new)
AC_CONFIG_SRCDIR([include/config.h])
AC_CONFIG_HEADERS([config.h])

# Specify the auxiliary build directory
AC_CONFIG_AUX_DIR([build])

# Initialize Automake.
AM_INIT_AUTOMAKE([gnu subdir-objects dist-xz])

# Initialize Libtool.
AC_CONFIG_MACRO_DIRS([m4])
AM_PROG_AR
LT_INIT

# Check for programs
AC_PROG_CC
AC_PROG_YACC
AC_PROG_MAKE_SET
AC_CHECK_TOOL([AR], [ar])
AC_CHECK_TOOL([LD], [ld])
AC_CHECK_TOOL([RANLIB], [ranlib])
AC_CHECK_PROG([GDB], [gdb], [gdb])
AC_CHECK_PROG([M4], [m4], [m4])

# Checks for libraries.
AC_CHECK_LIB([c], [malloc], [], [AC_MSG_ERROR([glibc is required])])
AC_CHECK_LIB([pthread], [pthread_create], [], [AC_MSG_ERROR([POSIX threads library is required])])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h string.h stdarg.h ctype.h sys/stat.h])

# Checks for typedefs
AC_TYPE_SIZE_T

# Checks for structures
AC_CHECK_HEADERS([sys/stat.h])

# Checks for compiler characteristics
AC_PROG_CC

# Checks for library functions.
AC_CHECK_FUNCS([memset strchr strdup strrchr strtol])

# Checks for system services
AC_CHECK_FUNCS([fork pipe])

# Specify directories for configuration files, source files, and header files
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    lib/Makefile
    include/Makefile
    etc/Makefile
    bin/Makefile
    obj/Makefile
    deps/Makefile
    docs/Makefile
    dist/Makefile
    logs/Makefile
    m4/Makefile
    tmp/Makefile
])

AC_OUTPUT
