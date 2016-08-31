TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SUBDIRS += gridlayout


INCLUDEPATH += /opt/local/include
LIBS += -L/opt/local/lib -lgtest


SOURCES += test_gridlayout.cpp \

HEADERS += ../../src/grid/gridlayout.h \
