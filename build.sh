#!/bin/bash

LIBTIFF_VERSION=4.0.3

PREFIX=`pwd`/local
mkdir ${PREFIX}

curl -O http://download.osgeo.org/libtiff/tiff-${LIBTIFF_VERSION}.tar.gz
tar xzvf tiff-${LIBTIFF_VERSION}.tar.gz
cd tiff-${LIBTIFF_VERSION}
patch -u libtiff/tiff.h < ../tiff_h_int_size.patch
emconfigure ./configure --prefix=${PREFIX}
emmake make
emmake make install
cd ../

emcc tiff2bmpsw.c \
     ./local/lib/libtiff.a \
     -I./local/include \
     -s EXPORTED_FUNCTIONS="['_tiff_to_bmp']" \
     --pre-js LICENSE \
     --pre-js tiff2bmpsw_pre.js \
     --js-library tiff2bmpsw_jslib.js \
     --post-js tiff2bmpsw_post.js \
     -o tiff2bmpsw.js

# Cleanup
rm -rf ${PREFIX} \
       tiff-${LIBTIFF_VERSION} \
       tiff-${LIBTIFF_VERSION}.tar.gz
