#
# Copyright (c) 2018      Los Alamos National Security, LLC
#                         All rights reserved.
#
# This file is part of the sdsdkv project. See the LICENSE file at the
# top-level directory of this distribution.
#

AC_PREREQ([2.60])

AC_INIT([sdsdkv],
        [0.1-alpha],
        [samuel@lanl.gov],
        [sdsdkv])
#dnl TODO(skg) add project URL.

AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_SRCDIR([src])

AX_SDSDKV_LIBTOOL_VERSION

################################################################################
# AC_CANONICAL_*
################################################################################
# Must be in between AC_INIT and AM_INIT_AUTOMAKE.
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

################################################################################
# Automake things
################################################################################
# note: -Wall -Werror are automake options.  they are NOT compiler flags
AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign])
# Build using AM_SILENT_RULES
AM_SILENT_RULES([yes])
AM_CONFIG_HEADER([config.h])

################################################################################
# C++ Language Support
################################################################################
AC_USE_SYSTEM_EXTENSIONS

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([], [mandatory])
AC_PROG_CXXCPP
AC_PROG_CXX_C_O

AX_PROG_MPICXX

AS_IF([test "x$AX_PROG_MPICXX_HAVE_MPICXX" != "x1"],dnl
      [AC_MSG_ERROR(
       [*** CXX cannot compile MPI apps. set CXX to something that can. ***])])

################################################################################
# Libtool things
################################################################################
AM_PROG_AR

LT_PREREQ([2.4.2])
LT_INIT

# checks for header files.
AC_CHECK_HEADERS([\
inttypes.h limits.h stdint.h stdlib.h string.h unistd.h stdbool.h time.h \
getopt.h ctype.h netdb.h sys/socket.h netinet/in.h arpa/inet.h sys/types.h \
stddef.h assert.h pthread.h sys/mman.h sys/stat.h fcntl.h syscall.h omp.h \
sched.h strings.h stdio.h errno.h math.h
])

# checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# checks for library functions.
AC_CHECK_FUNCS([memset strerror strtoul mmap])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT

# let the user know the configuration
cat << EOF

+-------------------------------------------------------------------------------
| sdsdkv Configuration :::::::::::::::::::::::::::::::::::::::::::::::::::::::
+-------------------------------------------------------------------------------
| version     : ${VERSION}
| target      : ${target}
| CXX         : ${CXX}
| CXXFLAGS    : ${CXXFLAGS}
| CPP         : ${CPP}
| CPPFLAGS    : ${CPPFLAGS}
| LDFLAGS     : ${LDFLAGS}
| LIBS        : ${LIBS}
+-------------------------------------------------------------------------------

EOF
