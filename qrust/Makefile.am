AC_AUX_DIR = m4
AC_MACRO_DIR = m4

EXTRA_DIST = $(top_srcdir)/bootstrap

SUBDIRS = display 
bin_PROGRAMS = qrust

INCLUDES = -I.. @QT_CXXFLAGS@

qrust_SOURCES = main.cpp
qrust_LDADD = 	display/libdisplay.a \
		@QT_LIBS@
