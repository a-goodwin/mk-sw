TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        camlist.cpp \
        cmdpacket.c \
        ctime.cpp \
        eventprocessor.cpp \
        gpio18.c \
        i2cpacket.cpp \
        i2cread.c \
        i2cwrite.c \
        kpn-ctrl.cpp \
        kpn_sock.c \
        kt-ctrl.cpp \
        queue.c \
        rs485test/rs485.c \
        sock.c \
        timer.cpp

INCLUDEPATH += /work/omega/source/staging_dir/target-mipsel_24kc_musl/usr/include

LIBS += -lugpio -lonioni2c -loniondebug

HEADERS += \
    camlist.h \
    cmdpacket.h \
    ctime.h \
    eventprocessor.h \
    globals.h \
    gpio18.h \
    i2cpacket.h \
    kpn_sock.h \
    queue.h \
    rs485test/rs485.h \
    sock.h \
    timer.h

SUBDIRS += \
    rs485test/rs485test.pro

DISTFILES += \
    inst.sh \
    makefile \
    xCompile.sh \
    xc.sh
