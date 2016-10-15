TEMPLATE=lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt



GTESTDIR = ../../googletest/googletest


LIBS        += -L$$GTESTDIR/lib -lgtest
INCLUDEPATH +=   $$GTESTDIR/include $$GTESTDIR


SOURCES = $$GTESTDIR/src/gtest-all.cc

