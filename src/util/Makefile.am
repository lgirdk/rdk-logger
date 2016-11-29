#####################################################################
#
# COMCAST PROPRIETARY AND CONFIDENTIAL
# __________________
#
#  Copyright 2015 Comcast Cable Communications Management, LLC
#  All Rights Reserved.
#
# NOTICE:  All information contained herein is, and remains the property of Comcast
# and its suppliers, if any.  The concepts contained herein are proprietary to Comcast
# and its suppliers and may be covered by U.S. and foreign patents or patents in process,
# and are protected by trade secret or copyright law.  Dissemination of this information
# or reproduction of this material is strictly forbidden without prior written permission
# from Comcast.
#
#####################################################################

SUBDIRS = 
AM_CPPFLAGS = -pthread -Wall
lib_LTLIBRARIES = libloggerutil.la
libloggerutil_la_SOURCES = rdk_dynamic_log_reg.c

INCLUDE_FILES = -I=$(includedir)/glib-2.0 \
                -I=$(libdir)/glib-2.0/include \
                -I=${includedir}/rdk/iarmbus \
                -I=${includedir}/rdk/rdklogctrl

libloggerutil_la_CFLAGS =  $(INCLUDE_FILES)
libloggerutil_la_LDFLAGS = $(GLIB_LIBS)  $(LOG4C_LIBS)
libloggerutil_la_CPPFLAGS =  $(GLIB_CFLAGS) -I$(top_srcdir)/src/include -I$(top_srcdir)/include
libloggerutil_la_LIBADD = -lIARMBus
