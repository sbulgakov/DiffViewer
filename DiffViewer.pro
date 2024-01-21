#-------------------------------------------------
#
# Project created by QtCreator 2024-01-14T16:13:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DiffViewer
TEMPLATE = app

SOURCES += main.cpp\
        diffview.cpp \
    diffhighlighter.cpp

HEADERS  += diffview.h \
    diffhighlighter.h
