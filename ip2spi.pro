TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cmdpacket.c \
        ctime.c \
        gpio18.c \
        i2cpacket.c \
        i2cread.c \
        i2cwrite.c \
        kpn-ctrl.c \
        kpn_sock.c \
        kt-ctrl.c \
        queue.c \
        rs485test/rs485.c \
        sock.c

INCLUDEPATH += /work/omega/source/staging_dir/target-mipsel_24kc_musl/usr/include

LIBS += -lugpio -lonioni2c -loniondebug

HEADERS += \
    cmdpacket.h \
    ctime.h \
    globals.h \
    gpio18.h \
    i2cpacket.h \
    kpn_sock.h \
    queue.h \
    rs485test/rs485.h \
    sock.h

SUBDIRS += \
    rs485test/rs485test.pro

DISTFILES += \
    inst.sh \
    makefile \
    xCompile.sh \
    xc.sh
