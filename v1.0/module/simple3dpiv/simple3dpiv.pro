#-------------------------------------------------
#
# Project created by QtCreator 2013-05-22T21:19:55
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = simple3dpiv
TEMPLATE = app

INCLUDEPATH+=D:\OpenCV2.4\mingwopencv\install\include
INCLUDEPATH+=D:\OpenCV2.4\mingwopencv\install\include\opencv
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_core240.dll.a
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_highgui240.dll.a
LIBS+=D:\OpenCV2.4\mingwopencv\install\lib\libopencv_imgproc240.dll.a

SOURCES += main.cpp\
    vector3d.cpp \
    vector_result_widget.cpp \
    settings.cpp \
    reconstruction_3dintensity.cpp \
    reconstruction_3d_from_tomedata.cpp \
    progressbar_widget.cpp \
    pivthread.cpp \
    pivengine.cpp \
    pivdata.cpp \
    mycvimage_widget.cpp \
    glwidget.cpp \
    generategrid.cpp \
    frame_3dpoint.cpp \
    datacontainer.cpp \
    processing.cpp \
    main_widget.cpp \
    normalizedcrosscorrelate.cpp \
    txt_writer.cpp

HEADERS  += \
    vector3d.h \
    vector_result_widget.h \
    settings.h \
    reconstruction_3dintensity.h \
    reconstruction_3d_from_tomedata.h \
    progressbar_widget.h \
    point3d.h \
    pivthread.h \
    pivengine.h \
    pivdata.h \
    mycvimage_widget.h \
    glwidget.h \
    generategrid.h \
    frame_3dpoint.h \
    datacontainer.h \
    processing.h \
    main_widget.h \
    normalizedcrosscorrelator.h \
    txt_writer.h

FORMS    += \
    vector_result_widget.ui \
    progressbar_widget.ui \
    main_widget.ui
