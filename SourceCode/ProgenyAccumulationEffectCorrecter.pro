#-------------------------------------------------
#
# Project created by QtCreator 2018-03-06T16:03:03
#
#-------------------------------------------------

QT       += core gui widgets printsupport

TARGET = 20180306_BG2015DataAnalysis
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    qcustomplot.cpp \
    DataAnalysis/DataSequence.cpp \
    DataAnalysis/DataAnalysis.cpp \
    SettingDialog.cpp

HEADERS  += MainWindow.h \
    qcustomplot.h \
    DataAnalysis/DataSequence.h \
    DataAnalysis/DataAnalysis.h \
    SettingDialog.h
