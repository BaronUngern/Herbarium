#-------------------------------------------------
#
# Project created by QtCreator 2014-10-24T10:30:14
#
#-------------------------------------------------

QT       += core gui sql

CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Herbarium
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addition.cpp \
    planttaxonomymodel.cpp

HEADERS  += mainwindow.h \
    addition.h \
    planttaxonomymodel.h

FORMS    += mainwindow.ui \
    addition.ui
