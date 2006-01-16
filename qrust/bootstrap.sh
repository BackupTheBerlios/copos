#! /bin/bash
# libtool-1.9f: 
LIBTOOLIZE='libtoolize --copy --automake' 
# libtool-1.5.18: 
# LIBTOOLIZE='libtoolize --copy --ltdl --automake' 
ACLOCAL='aclocal -I m4' 
AUTOHEADER='autoheader -f' 
AUTOCONF='autoconf' 
AUTOMAKE='automake --add-missing --copy'  
CONFIGURE='./configure --with-Qt-dir=/usr/lib/qt/' 
MAKE_CLEAN='make -s clean' 
MAKE_ALL='make -s all' 
#MAKE_INSTALL='make -s install'
#MAKE_DOXYGEN='make doxygen' 
#MAKE_DIST='make dist'
#MAKE_DISTCHECK='make distcheck'

for i in "$LIBTOOLIZE" "$ACLOCAL" "$AUTOHEADER" \
"$AUTOCONF" "$AUTOMAKE" "$CONFIGURE"; 
do
     if ! test -z "$i"; then
          echo                 
          echo "########### $i #############"        
          eval $i         
     fi 
done;

echo;
exit;