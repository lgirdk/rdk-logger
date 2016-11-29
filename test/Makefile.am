## Copyright (c) 2014 Comcast. All rights reserved.


SUBDIRS =
DIST_SUBDIRS =

bin_PROGRAMS = dynamicLoggerTest

dynamicLoggerTest_SOURCES=rdk_dynamic_logger_test.c
dynamicLoggerTest_LDADD = -L../src/util/.libs -lloggerutil -lIARMBus
dynamicLoggerTest_CPPFLAGS = -I$(top_srcdir)/include
dynamicLoggerTest_CFLAGS = -I./usr/include -I=/usr/include/rdk/iarmbus
dynamicLoggerTest_LDFLAGS = -I./usr/lib

