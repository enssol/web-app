AC_INIT([EnvEng Web Application],[1.0],[support@example.com])
AC_PREREQ([2.71])

# Specify the auxiliary build directory as 'build'
AC_CONFIG_AUX_DIR([build])

# Add Homebrew bin directory to PATH for macOS
case "$hostname" in
  agalloMacbook.local*)
    PATH="/opt/homebrew/bin:$PATH"
    ;;
esac

# Initialise Automake and Libtool
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_PROG_AR
LT_INIT

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_MACRO_DIRS([m4])


# Linker flags
LDFLAGS="-fuse-ld=mold -static -Wl,-z,relro,-z,now -L/opt/homebrew/lib -L/opt/homebrew/opt/curl/lib $GLIB_LDFLAGS"
AC_SUBST([LDFLAGS])
LDLIBS = -lm -lglib-2.0

# Check for required programs
AC_PROG_CC
AC_PROG_CPP
AC_PROG_YACC
AC_PROG_LEX([noyywrap])
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_CHECK_PROG([GDB], [gdb], [gdb])
AC_CHECK_PROG([DDD], [ddd], [ddd])
AC_CHECK_PROG([M4], [m4], [m4])
AC_CHECK_PROG([MAKE], [make], [make])
AC_CHECK_PROG([SED], [sed], [sed])
AC_CHECK_PROG([TAR], [tar], [tar])
AC_CHECK_PROG([YACC], [yacc], [yacc])
AC_CHECK_PROG([PKGCONF], [pkgconf], [pkgconf], [no])
AC_CHECK_PROG([SPLINT], [splint], [splint], [no])
AC_CHECK_PROG([CSCOPE], [cscope], [cscope], [no])
AC_CHECK_PROG([FLEX], [flex], [flex], [no])
AC_CHECK_PROG([BISON], [bison], [bison], [no])
AC_PATH_PROG([COMPLEXITY], [complexity], [/usr/bin/complexity], [no])
AC_CHECK_PROG([GAWK], [gawk], [gawk], [no])
AC_CHECK_PROG([INDENT], [indent], [indent], [no])
AC_CHECK_PROG([CLIB], [clib], [clib], [no])
AC_CHECK_PROG([RM], [rm], [rm], [no])
AC_CHECK_PROG([MKDIR], [mkdir], [mkdir], [no])
AC_CHECK_PROG([CP], [cp], [cp], [no])
AC_CHECK_PROG([MV], [mv], [mv], [no])
AC_CHECK_PROG([GPERF], [gperf], [gperf], [no])
AC_CHECK_PROG([CFLOW], [cflow], [cflow], [no])
AC_CHECK_PROG([GLOBAL], [global], [global], [no])
AC_CHECK_PROG([MAKEINFO], [makeinfo], [makeinfo], [no])
AC_CHECK_PROG([XZ], [xz], [xz], [no])
AC_CHECK_PROG([RECSEL], [recsel], [recsel], [no])
AC_CHECK_PROG([STOW], [stow], [stow], [no])
AC_CHECK_PROG([CLANG_FORMAT], [clang-format], [clang-format], [no])
AC_CHECK_PROG([CLANG_TIDY], [clang-tidy], [clang-tidy], [no])
AC_CHECK_PROG([CPPCHECK], [cppcheck], [cppcheck], [no])
AC_CHECK_PROG([SHELLCHECK], [shellcheck], [shellcheck], [no])
AC_CHECK_PROG([GCOV], [gcov], [gcov], [no])

# Check for ccache
CCACHE=ccache
AC_CHECK_PROG([CCACHE], [ccache], [ccache], [no])
if test "$CCACHE" != "no"; then
  CC="$CCACHE gcc"
  CXX="$CCACHE g++"
fi

# Otool path for macOS
AC_PATH_PROG(OTOOL, otool, /usr/bin/otool)
AC_SUBST(OTOOL)

# Library and header checks
AC_CHECK_LIB([m], [main])
AC_CHECK_HEADERS([stdio.h stdlib.h string.h stdarg.h ctype.h sys/stat.h])

# Set PKG_CONFIG_PATH for pkg-config to find glib-2.0
AC_SUBST([PKG_CONFIG_PATH], [$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig:/opt/homebrew/lib/pkgconfig])

# Checks for typedefs
AC_TYPE_SIZE_T

# Checks for system services
AC_CHECK_FUNCS([fork pipe])

# Check for required tools
AC_CHECK_TOOL([AR], [ar])
AC_CHECK_TOOL([LD], [mold], [ld])

# Check for required glib libraries
PKG_CHECK_MODULES([GLIB], [glib-2.0])

# Set the path to use your preferred ar
AC_SUBST([AR], [/opt/homebrew/Cellar/binutils/2.43.1/aarch64-apple-darwin24.1.0/bin/ar])

# Set the path to use your preferred nm
AC_SUBST([NM], [/opt/homebrew/Cellar/binutils/2.43.1/aarch64-apple-darwin24.1.0/bin/nm])

# Compiler flags
CFLAGS="-static -Iinclude -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I/opt/homebrew/include/glib-2.0 -I/opt/homebrew/lib/glib-2.0/include -O0 -pedantic -D_POSIX_C_SOURCE=2024L -D_XOPEN_SOURCE=800 -std=c17 -Wall -Wextra -Werror -DPIC -MMD -MP"
AC_SUBST([CFLAGS])

# No-op for MT
AC_SUBST([MT], [:])

# Configuration files
CONFIG_DIR="/etc/web-app"
CONFIG_INI_FILE="config.ini"
CONFIG_CONF_FILE="config.conf"
GCC_SPEC_FILE="gcc.spec"
AC_SUBST([CONFIG_DIR])
AC_SUBST([CONFIG_INI_FILE])
AC_SUBST([CONFIG_CONF_FILE])
AC_SUBST([GCC_SPEC_FILE])

# Configure files
AC_CONFIG_FILES([
    Makefile
    bin/Makefile
    build/Makefile
    deps/Makefile
    dist/Makefile
    docs/Makefile
    etc/Makefile
    include/Makefile
    lib/Makefile
    logs/Makefile
    m4/Makefile
    objects/Makefile
    po/Makefile
    src/Makefile
    tests/Makefile
    tmp/Makefile
])

# Add gtk-doc support
GTK_DOC_CHECK([1.25])

AC_OUTPUT
