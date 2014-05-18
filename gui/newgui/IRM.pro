#-------------------------------------------------
#
# Project created by QtCreator 2014-03-23T00:58:26
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IRM
TEMPLATE = app


SOURCES += main.cpp\
        gui.cpp \
    glwidget.cpp \
    camera.cpp

HEADERS  += gui.h \
    glwidget.h \
    camera.h

FORMS    += gui.ui
LIBS     += -lGLU -lassimp -lglut

OTHER_FILES +=
