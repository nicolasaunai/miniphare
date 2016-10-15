TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include(../GTest/GTest.pri)


INCLUDEPATH += ../../src


SOURCES += test_ions.cpp \
           test_species.cpp \
           ../../src/Plasmas/ions.cpp \
           ../../src/grid/gridlayout.cpp \
           ../../src/grid/gridlayoutimplinternals.cpp \
           ../../src/grid/gridlayoutimplyee.cpp \
           ../../src/Plasmas/species.cpp \
            ../../src/vecfield/vecfield.cpp \
            ../../src/Field/field.cpp \
            test_ions_chargedensity.cpp



HEADERS += ../../src/Plasmas/ions.h \
           ../../src/grid/gridlayout.h \
           ../../src/grid/gridlayoutimpl.h \
           ../../src/grid/gridlayoutimplinternals.h \
           ../../src/grid/gridlayoutimplyee.h \
           #../../src/Plasmas/species.h \
           ../../src/vecfield/vecfield.h \
           ../../src/Field/field.h \




