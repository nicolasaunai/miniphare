TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


#QMAKE_POST_LINK  = ls $$DESTDIR #python gridlayouttest.py $$DESTDIR


GTESTDIRLIB = ../gtest
GMOCKDIRLIB = ../gmock
GTESTINC    = ../../googletest/googletest/include
GMOCKINC    = ../../googletest/googlemock/include



LIBS +=  -L$$GTESTDIRLIB -lgtest  -L$$GMOCKDIRLIB -lgmock -pthread
INCLUDEPATH += $$GTESTINC $$GMOCKINC

INCLUDEPATH += ../../src




SOURCES += ../../src/grid/gridlayout.cpp \
           ../../src/grid/gridlayoutimplyee.cpp \
           ../../src/grid/gridlayoutimplinternals.cpp \
           ../../src/Field/field.cpp \
           test_indexing.cpp \
           test_gridlayout.cpp \
           test_fieldnodecoordinates.cpp \
           test_cellcenteredcoordinates.cpp \
           test_deriv1d.cpp \
           test_allocsizes.cpp \
           test_utilities.cpp \
           test_main.cpp \


HEADERS += ../../src/grid/gridlayout.h \
           ../../src/grid/gridlayoutimplfactory.h \
           ../../src/grid/gridlayoutimplyee.h \
           ../../src/grid/gridlayoutimpl.h \
           ../../src/grid/gridlayoutimplinternals.h \
           ../../src/types.h \
           ../../src/constants.h \
           ../../src/utility.h \
           ../../src/Field/field.h \
           test_gridlayout.h
