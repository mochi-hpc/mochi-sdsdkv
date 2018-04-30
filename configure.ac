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

# checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

################################################################################
# Header checks.
################################################################################
AC_CHECK_HEADERS([\
inttypes.h sys/types.h unistd.h stdint.h
])

################################################################################
# External package dependencies
################################################################################
# Sets SDSKV_SERVER_CFLAGS
#      SDSKV_SERVER_LIBS
PKG_CHECK_MODULES([SDSKV_SERVER],dnl
                  [sdskv-server],dnl
                  [], dnl
                  [AC_MSG_ERROR([sdskv-server pkg-config not found. ***])])

# Sets SDSKV_CLIENT_CFLAGS
#      SDSKV_CLIENT_LIBS
PKG_CHECK_MODULES([SDSKV_CLIENT],dnl
                  [sdskv-client],dnl
                  [], dnl
                  [AC_MSG_ERROR([sdskv-client pkg-config not found. ***])])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    tests/Makefile
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
