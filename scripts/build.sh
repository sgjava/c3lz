#!/bin/sh
#
# Created on February 27, 2024
#
# @author: sgoldsmith
#
# Make library source, create c3l.lib, compile demos and create disk images.
#
# Steven P. Goldsmith
# sgjava@gmail.com
#

# Project root (assumes starting in scripts dir)
cd ../
# Clean project
make clean
# Compile library source
make
# Create c3l.lib
make lib
# Need to explicitly set this
export ZCCCFG="$HOME/z88dk/lib/config"
# Add demo dir
mkdir ./build/demo

# Build demo apps
zcc +cpm -vn -DAMALLOC -I"$HOME/z88dk/include" -I./include -o ./build/demo/baseline.com ./src/demo/baseline.c
zcc +cpm -vn -I"$HOME/z88dk/include" -I./include -o ./build/demo/ciademo.com ./src/demo/ciademo.c -L./build/lib -lc3l
zcc +cpm -vn -I"$HOME/z88dk/include" -I./include -o ./build/demo/rtcdemo.com ./src/demo/rtcdemo.c -L./build/lib -lc3l
zcc +cpm -vn -I"$HOME/z88dk/include" -I./include -o ./build/demo/siddemo.com ./src/demo/siddemo.c -L./build/lib -lc3l
zcc +cpm -vn -DAMALLOC -I"$HOME/z88dk/include" -I./include -o ./build/demo/convpcm.com ./src/demo/convpcm.c -L./build/lib -lc3l
zcc +cpm -vn -DAMALLOC -I"$HOME/z88dk/include" -I./include -o ./build/demo/cpmdir.com ./src/demo/cpmdir.c -L./build/lib -lc3l

# Remove curent disk image
rm -f ./disks/demo.d71
# Create new disk image
cformat -2 ./disks/demo.d71
# Copy all com files to demo disk
ctools ./disks/demo.d71 p ./build/demo/*.com
# Copy all resource files to demo disk
ctools ./disks/demo.d71 p ./resources/*.*
