#!/bin/bash
OS=`uname -s`
ARCH=`uname -p`
if [ "$OS" == "Darwin" ]; then
  CC=clang
else
  CC=gcc
fi

CC_ARGS="-Wall -m64"

$CC $CC_ARGS randread.c -o randread.$OS-$ARCH 
