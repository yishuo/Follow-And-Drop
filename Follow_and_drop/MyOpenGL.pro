#-------------------------------------------------
#
# Project created by QtCreator 2013-09-25T09:11:42
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyOpenGL
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp \
    myglwidget.cpp \
    webcamwindow.cpp \
    fenetre_nom.cpp

HEADERS  += window.h \
    myglwidget.h \
    webcamwindow.h \
    fenetre_nom.h

INCLUDEPATH +=$$(OPENCV_DIR)\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core2411 \
    -lopencv_highgui2411 \
    -lopencv_imgproc2411 \
    -lopencv_features2d2411 \
    -lopencv_calib3d2411 \
    -lopencv_objdetect2411

FORMS    += window.ui

RESOURCES += \
    src.qrc \


