#
# Copyright (c) 2018      Los Alamos National Security, LLC
#                         All rights reserved.
#
# This file is part of the sdsdkv project. See the LICENSE file at the
# top-level directory of this distribution.
#

SUBDIRS =

lib_LTLIBRARIES = \
libsdsdkv.la

include_HEADERS = \
sdsdkv.h

libsdsdkv_la_SOURCES = \
sdsdkv-misci.h \
sdsdkv-config.h \
sdsdkv-mpi.h \
sdsdkv-personality.h \
sdsdkv-personality-factory.h \
sdsdkv-server.h \
sdsdkv-client.h \
sdsdkv-impl.h \
sdsdkv.cc

libsdsdkv_la_CFLAGS = \
@SSG_CFLAGS@ \
@CH_PLACEMENT_CFLAGS@ \
@SDSKV_CLIENT_CFLAGS@ \
@SDSKV_SERVER_CFLAGS@

libsdsdkv_la_LIBADD = \
@SSG_LIBS@ \
@CH_PLACEMENT_LIBS@ \
@SDSKV_CLIENT_LIBS@ \
@SDSKV_SERVER_LIBS@

libsdsdkv_la_LDFLAGS = \
-version-info @SDSDKV_LIBVINFO@
