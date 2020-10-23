TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        i2cpacket.c \
        i2cread.c \
        main.c \
        queue.c

INCLUDEPATH += /work/omega/source/staging_dir/target-mipsel_24kc_musl/usr/include

LIBS += -lugpio -lonioni2c -loniondebug

HEADERS += \
    globals.h \
    i2cpacket.h \
    queue.h

