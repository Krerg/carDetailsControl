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
    deletedetailwindow.cpp \
    excelhandler.cpp \
    changearticlewindow.cpp \
    importfromexcelwindow.cpp \
    renamecarmakewindow.cpp \
    renamedetailcategory.cpp \
    renamedetailwindow.cpp \
    updatingwindow.cpp \
    abstractrenamewindow.cpp

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
    deletedetailwindow.h \
    excelhandler.h \
    changearticlewindow.h \
    importfromexcelwindow.h \
    renamecarmakewindow.h \
    renamedetailcategory.h \
    renamedetailwindow.h \
    updatingwindow.h \
    abstractrenamewindow.h

FORMS    += mainwindow.ui
include(3rdparty\QtXlsxWriter-master\src\xlsx\qtxlsx.pri)
