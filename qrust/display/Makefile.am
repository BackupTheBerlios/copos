ACLOCAL_AMFLAGS = -I m4
all:
	QTDIR=$(QT_DIR)
	@QMAKE@ -makefile -o Makefile.qt qrust_display.pro
	make -f Makefile.qt
clean:
	QTDIR=$(QT_DIR)
	@QMAKE@ -makefile -o Makefile.qt qrust_display.pro
	make -f Makefile.qt clean