#!/bin/sh

cd /work/omega/ip2c
sh /work/omega/ip2c/_xCompile.sh -buildroot /work/omega/source -lib "ugpio,onioni2c,oniondeug"

echo "\r\nCopy files to Omega"
cp kt-ctrl ../sysroot/tmp/kt-ctrl
cp kpn-ctrl ../sysroot/tmp/kpn-ctrl
cp i2cread ../sysroot/tmp/i2cread
cp i2cwrite ../sysroot/tmp/i2cwrite
cp inst.sh ../sysroot/tmp/inst.sh
