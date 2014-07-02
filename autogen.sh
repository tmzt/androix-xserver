#! /bin/sh

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd $srcdir

autoreconf -I /ndk/androix/usr/share/aclocal --force -v --install || exit 1
cd $ORIGDIR || exit $?

$srcdir/configure --enable-maintainer-mode "$@"
