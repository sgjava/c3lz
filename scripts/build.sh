cd ../
make clean
make
make lib
export ZCCCFG="$HOME/z88dk/lib/config"
mkdir ./build/demo
zcc +cpm -I/home/sgoldsmith/z88dk/include -I./include -o ./build/demo/ciademo.com ./src/demo/ciademo.c -L./build/lib -lc3l
rm -f ./disks/demo.d71
cformat -2 ./disks/demo.d71
ctools ./disks/demo.d71 p ./build/demo/*.com
