#!/bin/sh

CXXFLAGS="$CXXFLAGS $CVMFS_PATCHED_CXXFLAGS" cmake .
