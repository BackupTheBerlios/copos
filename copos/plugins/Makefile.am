if TEST_PWC
MODULE=libphilips740k.la
else
MODULE=
endif

noinst_LTLIBRARIES = libgenericv4l.la $(MODULE)

noinst_LTHEADERS = $(wildcard *.h)

INCLUDES = -I.. @GLIB2_CFLAGS@ @GTK2_CFLAGS@ @PWC_CFLAGS@

libgenericv4l_la_SOURCES = genericv4l.c  genericv4l.h webcam.h
libgenericv4l_la_LIBADD  = @GLIB2_LIBS@
libgenericv4l_la_LDFLAGS = -rpath $(pkgdatadir) -avoid-version -module

libphilips740k_la_SOURCES = 	philips740k.c  philips740k.h  webcam.h \
				confphilips.c confphilips.h \
				confsizefrm.c confsizefrm.h \
				confvidctrl.c confvidctrl.h \
				confextensions.c confextensions.h \
libphilips740k_la_LIBADD  = 	@GTK2_LIBS@
libphilips740k_la_LDFLAGS = -rpath $(pkgdatadir) -avoid-version -module
