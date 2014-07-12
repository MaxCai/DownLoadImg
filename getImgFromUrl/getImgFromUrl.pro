TEMPLATE = app

TARGET = 
CONFIG += console
CONFIG += debug

INCLUDEPATH += $$(QTDIR)/include/QtNetwork

LIBS += -L$${QTDIR}/lib -lQtNetworkd4

HEADERS += Manager.h

SOURCES += Manager.cpp