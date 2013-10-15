#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T21:14:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ET
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    newsolutiondialog.cpp \
    welcomepagewidget.cpp \
	AST.cpp \
    myinputpanelcontext.cpp \
    myinputpanel.cpp

HEADERS  += mainwindow.hpp \
    newsolutiondialog.hpp \
    welcomepagewidget.hpp \
    AST.hpp \
    symbol.hpp \
    myinputpanelcontext.hpp \
    myinputpanel.hpp

FORMS    += mainwindow.ui \
    newsolutiondialog.ui \
    welcomepagewidget.ui \
    myinputpanel.ui
