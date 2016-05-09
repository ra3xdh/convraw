#!/bin/sh

PKG=spiceraw
VERSION=0.1
FULL_PKG=$PKG-$VERSION.tar.gz

TMP=/tmp/

WORKDIR=$PKG-$VERSION

FILES="COPYING INDEX DESCRIPTION"
DEP_SOURCES="../convraw.cpp ../convraw.h"
SOURCES="Makefile read_spiceraw.cpp"

echo "Creating Octave package $FULL_PKG ..."

mkdir $TMP$WORKDIR
mkdir $TMP$WORKDIR/src
mkdir $TMP$WORKDIR/inst

cp $FILES $DEP_SOURCES $TMP$WORKDIR
cp $SOURCES $TMP$WORKDIR/src

cd $TMP
tar cvfz $FULL_PKG $WORKDIR

if [ -f $FULL_PKG ]; then
	echo "Octave package $FULL_PKG successfully created"
	echo "Package path is: $TMP$FULL_PKG"
	rm -rf $TMP$WORKDIR
else
	echo "Package creation failed"
fi
