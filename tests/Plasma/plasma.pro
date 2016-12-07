TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include(../GTest/GTest.pri)


INCLUDEPATH += ../../src


SOURCES += ../../src/Plasmas/ions.cpp \
           ../../src/Plasmas/species.cpp \
           ../../src/grid/gridlayout.cpp \
           ../../src/grid/gridlayoutimplinternals.cpp \
           ../../src/grid/gridlayoutimplyee.cpp \
           ../../src/Interpolator/interpolator.cpp \
           ../../src/Projector/projector.cpp \
           ../../src/IndexesAndWeights/indexesandweights.cpp \
           ../../src/vecfield/vecfield.cpp \
           ../../src/Field/field.cpp \
           test_ions.cpp \
           test_species.cpp \
           test_ions_chargedensity.cpp



HEADERS += ../../src/Plasmas/ions.h \
           ../../src/Plasmas/species.h \
           ../../src/grid/gridlayout.h \
           ../../src/grid/gridlayoutimpl.h \
           ../../src/grid/gridlayoutimplinternals.h \
           ../../src/grid/gridlayoutimplyee.h \
           ../../src/Interpolator/interpolator.h \
           ../../src/Projector/projector.h \
           ../../src/IndexesAndWeights/indexesandweights.h \
           ../../src/vecfield/vecfield.h \
           ../../src/Field/field.h




