#!/bin/sh
OS=`uname -s`
ARCH=`uname -p`
if [ $OS == Darwin ]; then
  CC=clang
else
  CC=gcc
fi


$CC randread.c -o randread.$OS-$ARCH 
