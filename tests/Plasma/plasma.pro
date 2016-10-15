TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt



GTESTDIRLIB = ../gtest
GMOCKDIRLIB = ../gmock
GTESTINC    = ../../googletest/googletest/include
GMOCKINC    = ../../googletest/googlemock/include


macx {
    LIBS +=  -L$$GTESTDIRLIB -lgtest  -L$$GMOCKDIRLIB -lgmock
    INCLUDEPATH += $$GTESTINC $$GMOCKINC
} else {
unix {
    #LIBS += -L/usr/lib -lpthread -lgtest -lgmock
    #INCLUDEPATH += /usr/include/gtest
    #INCLUDEPATH += /usr/include/gmock
}
}

INCLUDEPATH += ../../srck


SOURCES += test_ions.cpp \
           test_species.cpp \
           ../../src/Plasmas/ions.cpp \
           ../../src/grid/gridLayout.cpp \
           ../../src/grid/gridLayoutimplinternals.cpp \
           ../../src/grid/gridLayoutimplyee.cpp \
           ../../src/Plasmas/species.cpp \
            ../../src/vecfield/vecfield.cpp \
            ../../src/Field/field.cpp \
            test_ions_chargedensity.cpp



HEADERS += ../../src/Plasmas/ions.h \
           ../../src/grid/gridLayout.h \
           ../../src/grid/gridLayoutimpl.h \
           ../../src/grid/gridLayoutimplinternals.h \
           ../../src/grid/gridLayoutimplyee.h \
           #../../src/Plasmas/species.h \
           ../../src/vecfield/vecfield.h \
           ../../src/Field/field.h \




