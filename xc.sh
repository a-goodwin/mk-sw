#!/bin/sh

cd /work/omega/ip2spi
sh /work/omega/ip2spi/xCompile.sh -buildroot /work/omega/source -lib ugpio
cp main ../sysroot/tmp/main
