TEMPLATE = lib
CONFIG += staticlib
CONFIG += debug_and_release

TARGET = SEfile

INCLUDEPATH += ./
win32 {
    DEFINES -= UNICODE \
        _UNICODE
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

linux-g++ {
    DEFINES += _GNU_SOURCE
}

HEADERS += \
    se3/sha256.h \
    se3/se3comm.h \
    se3/se3c1def.h \
    se3/se3c0def.h \
    se3/se3_common.h \
    se3/pbkdf2.h \
    se3/L1.h \
    se3/L0.h \
    se3/crc16.h \
    se3/aes256.h \
    SEfile.h


SOURCES += \
    se3/sha256.c \
    se3/aes256.c \
    se3/crc16.c \
    se3/L0.c \
    se3/L1.c \
    se3/se3_common.c \
    se3/se3comm.c \
    se3/pbkdf2.c \
    SEfile.c
