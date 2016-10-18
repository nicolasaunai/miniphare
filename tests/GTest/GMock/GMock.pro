TEMPLATE=lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
TARGET = gmock

GTESTDIR = ../../../googletest/googletest
GMOCKDIR = ../../../googletest/googlemock
#LIBS        += -L$$GMOCKDIR/lib -lgmock
INCLUDEPATH +=   $$GMOCKDIR/include $$GMOCKDIR $$GTESTDIR/include

SOURCES = $$GMOCKDIR/src/gmock-all.cc

DISTFILES += \
    gmock.pri