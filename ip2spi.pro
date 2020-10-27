TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        i2cpacket.c \
        i2cread.c \
        i2cwrite.c \
        main.c \
        queue.c \
        rs485test/rs485.c \
        rs485test/rs485usb.cpp \
        sock.c

INCLUDEPATH += /work/omega/source/staging_dir/target-mipsel_24kc_musl/usr/include

LIBS += -lugpio -lonioni2c -loniondebug

HEADERS += \
    globals.h \
    i2cpacket.h \
    queue.h \
    rs485test/rs485.h \
    rs485test/rs485usb.h \
    sock.h

SUBDIRS += \
    rs485test/rs485test.pro
