#!/bin/sh

cd /work/omega/ip2c
sh /work/omega/ip2c/xCompile.sh -buildroot /work/omega/source -lib "ugpio,onioni2c,oniondeug"
cp main ../sysroot/tmp/main
cp i2cread ../sysroot/tmp/i2cread
