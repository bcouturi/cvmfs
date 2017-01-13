#!/bin/sh

sh configure CFLAGS="$CFLAGS $CVMFS_EXTERNAL_C_FLAGS -D_FILE_OFFSET_BITS=64" \
  CXXFLAGS="$CXXFLAGS $CVMFS_EXTERNAL_CXX_FLAGS -D_FILE_OFFSET_BITS=64" \
  --disable-dependency-tracking
