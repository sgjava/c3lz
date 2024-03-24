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
zcc +cpm -vn -DAMALLOC -I"$HOME/z88dk/include" -I./include -o ./build/demo/baseline.com ./src/demo/baseline.c -L./build/lib -lc3l
zcc +cpm -vn -I"$HOME/z88dk/include" -I./include -o ./build/demo/ciademo.com ./src/demo/ciademo.c -L./build/lib -lc3l
zcc +cpm -vn -I"$HOME/z88dk/include" -I./include -o ./build/demo/rtcdemo.com ./src/demo/rtcdemo.c -L./build/lib -lc3l
zcc +cpm -vn -I"$HOME/z88dk/include" -I./include -o ./build/demo/siddemo.com ./src/demo/siddemo.c -L./build/lib -lc3l
zcc +cpm -vn -DAMALLOC -I"$HOME/z88dk/include" -I./include -o ./build/demo/convpcm.com ./src/demo/convpcm.c -L./build/lib -lc3l
zcc +cpm -vn -DAMALLOC -I"$HOME/z88dk/include" -I./include -o ./build/demo/cpmdir.com ./src/demo/cpmdir.c -L./build/lib -lc3l
zcc +cpm -vn -DAMALLOC -I"$HOME/z88dk/include" -I./include -o ./build/demo/playpcm.com ./src/demo/playpcm.c -L./build/lib -lc3l
zcc +cpm -vn -DAMALLOC -I"$HOME/z88dk/include" -I./include -o ./build/demo/compile.com ./src/demo/compile.c -L./build/lib -lc3l

# Remove curent demo disk image
rm -f ./disks/demo.d71
# Create new demo disk image
cformat -2 ./disks/demo.d71
# Copy all com files to demo disk
ctools ./disks/demo.d71 p ./build/demo/*.com
# Copy resource files to demo disk
ctools ./disks/demo.d71 p ./resources/test.sub

# Remove curent playpcm disk image
rm -f ./disks/playpcm.d71
# Create new demo disk image
cformat -2 ./disks/playpcm.d71
# Copy resource files to demo disk
ctools ./disks/playpcm.d71 p ./resources/*.snd
# Copy com files to playpcm disk
ctools ./disks/playpcm.d71 p ./build/demo/baseline.com
ctools ./disks/playpcm.d71 p ./build/demo/convpcm.com
ctools ./disks/playpcm.d71 p ./build/demo/playpcm.com
