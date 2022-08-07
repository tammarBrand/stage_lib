QT -= gui

TEMPLATE = lib
DEFINES += STAGE_LIB_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS+= -L/usr/lib/x86_64-linux-gnu/libjpeg.so -ljpeg
SOURCES += \
    Queue.c \
    capture.c \
    encode.c \
    functions_tools.c \
    main.c \
    rgb_converter.c \
    save_snapshot.c \
    stage_lib.cpp \
    write_record.c \
    yuv_converter.c

HEADERS += \
    Queue.h \
    capture.h \
    encode.h \
    functions_tools.h \
    rgb_converter.h \
    save_snapshot.h \
    stage_lib.h\
    jpeglib.h \
    stage_lib_global.h \
    write_record.h \
    yuv_converter.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    stage_lib.pro.user
