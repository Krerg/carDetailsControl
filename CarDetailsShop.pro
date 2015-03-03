#-------------------------------------------------
#
# Project created by QtCreator 2015-01-23T14:49:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarDetailsShop
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    createcarmakewindow.cpp \
    confirmwindow.cpp \
    createcarmodelwindow.cpp \
    createdetailcategorywindow.cpp \
    deletedetailcategorywindow.cpp \
    settingswindow.cpp \
    createarticlewindow.cpp \
    deletearticlewindow.cpp \
    createdetailwindow.cpp \
    deletedetailwindow.cpp

HEADERS  += mainwindow.h \
    createcarmakewindow.h \
    confirmwindow.h \
    createcarmodelwindow.h \
    createdetailcategorywindow.h \
    deletedetailcategorywindow.h \
    settingswindow.h \
    createarticlewindow.h \
    deletearticlewindow.h \
    createdetailwindow.h \
    deletedetailwindow.h

FORMS    += mainwindow.ui
include(3rdparty\QtXlsxWriter-master\src\xlsx\qtxlsx.pri)
