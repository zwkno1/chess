#-------------------------------------------------
#
# Project created by QtCreator 2016-05-01T20:43:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chess
TEMPLATE = app


SOURCES += main.cpp \
    chess.cpp \
    chesswidget.cpp \
    mainwindow.cpp

HEADERS  += \
    chess.h \
    chesswidget.h \
    mainwindow.h

QMAKE_CXXFLAGS += -std=c++14

RESOURCES += \
    pic.qrc

FORMS += \
    mainwindow.ui
