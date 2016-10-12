TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SUBDIRS += GridLayout


# if macx else unix
macx {
    LIBS += -L/usr/local/lib -lgtest -lgmock  #-L/opt/local/lib -lgtest -lgmock
    INCLUDEPATH += /usr/local/include #/opt/local/include
} unix {
    LIBS += -L/usr/lib -lpthread -lgtest -lgmock
    INCLUDEPATH += /usr/include/gtest
    INCLUDEPATH += /usr/include/gmock
}

INCLUDEPATH += ../../src


#    QMAKE_CXXFLAGS +=  -Werror=constant-conversion



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
           test_main.cpp

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
