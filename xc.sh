#!/bin/sh

cd /work/omega/ip2c
sh /work/omega/ip2c/xCompile.sh -buildroot /work/omega/source -lib "ugpio,onioni2c,oniondeug"

echo "\r\nCopy files to Omega"
cp main ../sysroot/tmp/main
cp i2cread ../sysroot/tmp/i2cread
cp i2cwrite ../sysroot/tmp/i2cwrite
