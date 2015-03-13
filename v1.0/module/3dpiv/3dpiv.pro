#-------------------------------------------------
#
# Project created by QtCreator 2013-05-01T09:43:45
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = 3dpiv
TEMPLATE = app


SOURCES += main.cpp\
        window_3dpiv.cpp \
        pivthread.cpp \
        pivengine.cpp \
        pivdisplay.cpp \
        pivdata.cpp \
        settings.cpp \
        datacontainer.cpp \
        filteroptions.cpp \
        gaussiansubpixel.cpp \
        itempaint.cpp \
        generategrid.cpp

HEADERS  += window_3dpiv.h \
        pivthread.h \
        pivengine.h \
        pivdisplay.h \
        pivdata.h \
        settings.h \
        datacontainer.h \
        filteroptions.h \
        gaussiansubpixel.h \
        itempaint.h \
        generategrid.h

FORMS    += window_3dpiv.ui
