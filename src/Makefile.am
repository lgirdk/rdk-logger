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

SUBDIRS = $(UTILS)
AM_CPPFLAGS = -pthread -Wall
lib_LTLIBRARIES = librdkloggers.la
librdkloggers_la_SOURCES = rdk_debug.c rdk_debug_priv.c rdk_logger_init.c rdk_logger_util.c
librdkloggers_la_CFLAGS =  $(GLIB_CFLAGS) $(SYSTEMD_CFLAGS) -I./include -I../include -I${RDK_PROJECT_ROOT_PATH}/opensource/include/log4c -I${RDK_PROJECT_ROOT_PATH}/opensource/include -DDEBUG_CONF_FILE="\"debug.ini\""
librdkloggers_la_LDFLAGS = $(GLIB_LIBS)  $(LOG4C_LIBS) $(SYSTEMD_LDFLAGS)
librdkloggers_la_CPPFLAGS =  $(GLIB_CFLAGS) -I$(top_srcdir)/src/include -I$(top_srcdir)/include
