TEMPLATE=lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
TARGET = gtest


GTESTDIR = ../../../googletest/googletest


#LIBS        += -L$$GTESTDIR/lib -lgtest
INCLUDEPATH +=   $$GTESTDIR/include $$GTESTDIR


SOURCES = $$GTESTDIR/src/gtest-all.cc

