#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Autoconf configuration file for the web-app project.
# Information about the project is stored in the `configure.ac` file.
AC_PREREQ([2.71])
AC_INIT([web-app],[1.0],[https://github.com/enssol/web-app/issues/new])
AC_CONFIG_SRCDIR([include/config.h])
AC_CONFIG_HEADERS([config.h])

# Add this at the top of the file to define the new option
AC_ARG_ENABLE([gettext],
    [AS_HELP_STRING([--disable-gettext], [Disable gettext support])],
    [enable_gettext=$enableval], [enable_gettext=yes])

# Source the .env file if it exists
if test -f .env; then
  . ./.env
fi

# Specify the auxiliary build directory
AC_CONFIG_AUX_DIR([build])

# Initialize Automake and Libtool
AM_INIT_AUTOMAKE([gnu subdir-objects dist-xz])
AC_CONFIG_MACRO_DIRS([m4])
AM_PROG_AR
LT_INIT

# Linker flags
LDFLAGS="-fuse-ld=mold -static -Wl,-z,relro,-z,now -L/opt/homebrew/lib -L/opt/homebrew/opt/curl/lib"
LDFLAGS=$(echo $LDFLAGS | sed 's/-single_module//g')
AC_SUBST([LDFLAGS])


# Check for required programs
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_YACC
AC_PROG_LEX([noyywrap])
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_CHECK_TOOL([AR], [ar])
AC_CHECK_TOOL([LD], [mold])
AC_CHECK_PROG([GDB], [gdb], [gdb])
AC_CHECK_PROG([DDD], [ddd], [ddd])
AC_CHECK_PROG([M4], [m4], [m4])
AC_CHECK_PROG([MAKE], [make], [make])
AC_CHECK_PROG([SED], [sed], [sed])
AC_CHECK_PROG([TAR], [tar], [tar])
AC_CHECK_PROG([YACC], [yacc], [yacc])

# Check for pkgconf
AC_CHECK_PROG([PKGCONF], [pkgconf], [pkgconf], [no])
if test "$PKGCONF" = "no"; then
  AC_MSG_ERROR([pkgconf is required])
fi

# Use pkgconf to find libraries
PKG_CHECK_MODULES([LIBS], [glib-2.0])

# Set PKG_CONFIG_PATH for pkg-config to find glib-2.0
AC_SUBST([PKG_CONFIG_PATH], [$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig:/opt/homebrew/lib/pkgconfig])

# Check for splint
AC_CHECK_PROG([SPLINT], [splint], [splint], [no])
if test "$SPLINT" = "no"; then
  AC_MSG_ERROR([splint is required])
fi

# Check for cscope
AC_CHECK_PROG([CSCOPE], [cscope], [cscope], [no])
if test "$CSCOPE" = "no"; then
  AC_MSG_ERROR([cscope is required])
fi

# Check for flex and bison
AC_CHECK_PROG([FLEX], [flex], [flex], [no])
if test "$FLEX" = "no"; then
  AC_MSG_ERROR([flex is required])
fi
AC_CHECK_PROG([BISON], [bison], [bison], [no])
if test "$BISON" = "no"; then
  AC_MSG_ERROR([bison is required])
fi

# Check for other required tools
AC_PATH_PROG([COMPLEXITY], [complexity], [/usr/bin/complexity], [no])
if test "$COMPLEXITY" = "no"; then
  AC_MSG_ERROR([gnu-complexity is required])
fi

# Check for gawk
AC_CHECK_PROG([GAWK], [gawk], [gawk], [no])
if test "$GAWK" = "no"; then
  AC_MSG_ERROR([gawk is required])
fi

# Check for autoindent
AC_CHECK_PROG([INDENT], [indent], [indent], [no])
if test "$INDENT" = "no"; then
  AC_MSG_ERROR([indent is required])
fi

# Check for clib
AC_CHECK_PROG([CLIB], [clib], [clib], [no])
if test "$CLIB" = "no"; then
  AC_MSG_ERROR([clib is required])
fi

# Install clib dependencies
AC_CONFIG_COMMANDS([clib_install], [
  echo "Installing clib dependencies..."
  clib install
])

# Check for ccache
AC_CHECK_PROG([CCACHE], [ccache], [ccache], [no])
if test "$CCACHE" != "no"; then
  CC="ccache gcc"
  CXX="ccache g++"
fi

# Check for binutils
AC_CHECK_TOOL([AR], [ar])
AC_CHECK_TOOL([LD], [ld])

# Set the path to use your preferred ar
AC_SUBST([AR], [/opt/homebrew/Cellar/binutils/2.43.1/aarch64-apple-darwin24.1.0/bin/ar])

# Set the path to use your preferred nm
AC_SUBST([NM], [/opt/homebrew/Cellar/binutils/2.43.1/aarch64-apple-darwin24.1.0/bin/nm])

# Check for coreutils
AC_CHECK_PROG([RM], [rm], [rm], [no])
AC_CHECK_PROG([MKDIR], [mkdir], [mkdir], [no])
AC_CHECK_PROG([CP], [cp], [cp], [no])
AC_CHECK_PROG([MV], [mv], [mv], [no])

# No-op for MT
AC_SUBST([MT], [:])

# Check for valgrind
AC_CHECK_PROG([VALGRIND], [valgrind], [valgrind], [no])
if test "$VALGRIND" = "no"; then
  AC_MSG_WARN([valgrind is not found. Memory leak checks will be skipped.])
fi

# Add Homebrew bin directory to PATH for macOS
case "$host_os" in
  darwin*)
    PATH="/opt/homebrew/bin:$PATH"
    ;;
esac

# Check for gnu-perf (gperf)
AC_CHECK_PROG([GPERF], [gperf], [gperf], [no])
if test "$GPERF" = "no"; then
  AC_MSG_ERROR([gnu-perf (gperf) is required])
fi

# Check for cflow
AC_CHECK_PROG([CFLOW], [cflow], [cflow], [no])
if test "$CFLOW" = "no"; then
  AC_MSG_ERROR([cflow is required])
fi

# Check for GNU Global
AC_CHECK_PROG([GLOBAL], [global], [global], [no])
if test "$GLOBAL" = "no"; then
  AC_MSG_ERROR([global is required])
fi

# Check for texinfo
AC_CHECK_PROG([MAKEINFO], [makeinfo], [makeinfo], [no])
if test "$MAKEINFO" = "no"; then
  AC_MSG_ERROR([texinfo is required])
fi

# Check for xz
AC_CHECK_PROG([XZ], [xz], [xz], [no])
if test "$XZ" = "no"; then
  AC_MSG_ERROR([xz is required])
fi

# Check for recutils (recsel)
AC_CHECK_PROG([RECSEL], [recsel], [recsel], [no])
if test "$RECSEL" = "no"; then
  AC_MSG_ERROR([recutils is required])
fi

# Check for stow
AC_CHECK_PROG([STOW], [stow], [stow], [no])
if test "$STOW" = "no"; then
  AC_MSG_ERROR([stow is required])
fi

# Check for autogen
AC_CHECK_PROG([AUTOGEN], [autogen], [autogen], [no])
if test "$AUTOGEN" = "no"; then
  AC_MSG_ERROR([autogen is required])
fi

# Check for clang-format
AC_CHECK_PROG([CLANG_FORMAT], [clang-format], [clang-format], [no])
if test "$CLANG_FORMAT" = "no"; then
  AC_MSG_ERROR([clang-format is required])
fi

# Check for clang-tidy
AC_CHECK_PROG([CLANG_TIDY], [clang-tidy], [clang-tidy], [no])
if test "$CLANG_TIDY" = "no"; then
  AC_MSG_WARN([clang-tidy is not found. Code quality checks will be skipped.])
fi

# Check for cppcheck
AC_CHECK_PROG([CPPCHECK], [cppcheck], [cppcheck], [no])
if test "$CPPCHECK" = "no"; then
  AC_MSG_ERROR([cppcheck is required])
fi

# Check for shellcheck
AC_CHECK_PROG([SHELLCHECK], [shellcheck], [shellcheck], [no])
if test "$SHELLCHECK" = "no"; then
  AC_MSG_ERROR([shellcheck is required])
fi

# Check for gcov
AC_CHECK_PROG([GCOV], [gcov], [gcov], [no])
if test "$GCOV" = "no"; then
  AC_MSG_ERROR([gcov is required])
fi

# Ensure otool is available
AC_PATH_PROG(OTOOL, otool, /usr/bin/otool)
if test -z "$OTOOL"; then
  AC_MSG_ERROR([otool is required but not found])
fi
AC_SUBST(OTOOL)

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h string.h stdarg.h ctype.h sys/stat.h])

# Checks for typedefs
AC_TYPE_SIZE_T

# Checks for system services
AC_CHECK_FUNCS([fork pipe])

# Specify directories for configuration files, source files, and header files
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    lib/Makefile
    include/Makefile
    po/Makefile
    etc/Makefile
    bin/Makefile
    objects/Makefile
    deps/Makefile
    docs/Makefile
    dist/Makefile
    logs/Makefile
    m4/Makefile
    tmp/Makefile
])

# Add gtk-doc support
GTK_DOC_CHECK([1.25])

AC_SUBST([APP_NAME])
AC_SUBST([VERSION])
AC_SUBST([APP_MODE])
AC_SUBST([SRC_DIR])
AC_SUBST([BUILD_DIR])
AC_SUBST([OBJ_DIR])
AC_SUBST([BIN_DIR])
AC_SUBST([INCLUDE_DIR])
AC_SUBST([CONFIG_DIR])
AC_SUBST([LOGS_DIR])
AC_SUBST([DOCS_DIR])
AC_SUBST([TMP_DIR])
AC_SUBST([TESTS_DIR])
AC_SUBST([DIST_DIR])
AC_SUBST([LIB_DIR])
AC_SUBST([DEPS_DIR])
AC_SUBST([MACROS_DIR])
AC_SUBST([LOCALES_DIR])

AC_OUTPUT
