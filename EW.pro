QT += core
QT -= gui

CONFIG += c++11

TARGET = EW
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    EWDataSource.cpp \
    EWModel.cpp \
    EWTheme.cpp

HEADERS += \
    EWDataSource.h \
    EWModel.h \
    EWTheme.h
