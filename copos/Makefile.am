SUBDIRS = po engine display plugins 
bin_PROGRAMS = copos

INCLUDES = -I.. @GTKGL_CFLAGS@

copos_SOURCES = global.c global.h main.c 
copos_LDADD = engine/libengine.a \
		display/libdisplay.a \
		@GTKGL_LIBS@

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/plugins/.libs ; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir); \
	  for lib in $(srcdir)/plugins/.libs/*.so; do \
	    if test -f $$lib; then \
	      $(INSTALL_DATA) $$lib $(DESTDIR)$(pkgdatadir); \
	    fi \
	  done \
	fi
	if test -d $(srcdir)/pixmaps ; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir); \
	  for pix in $(srcdir)/pixmaps/*; do \
	    if test -f $$pix; then \
	      $(INSTALL_DATA) $$pix $(DESTDIR)$(pkgdatadir); \
	    fi \
	  done \
	fi

uninstall-local:
	@$(NORMAL_INSTALL)
	if test -d $(DESTDIR)$(pkgdatadir); then \
	  rm -rf $(DESTDIR)$(pkgdatadir); \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi
