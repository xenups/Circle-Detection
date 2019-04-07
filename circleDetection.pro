#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T22:49:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = circleDetection
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_features2d -lopencv_imgproc
