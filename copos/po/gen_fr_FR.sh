#!/bin/sh

( xgettext -V) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`xgettext' installed."
  echo "Download the appropriate package for your distribution,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
  DIE=1
}

xgettext --default-domain=copos -o copos.pot --directory=.. \
    --add-comments --keyword=_ --keyword=N_ \
    --files-from=./POTFILES.in --from-code=ISO-8859-15
msginit -l fr_FR -o fr_FR.po -i copos.pot