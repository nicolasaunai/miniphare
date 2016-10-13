TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#SUBDIRS += Plasma


INCLUDEPATH += /opt/local/include \
               ../../src



# if macx else unix
macx {
    LIBS += -L/opt/local/lib -lgtest
} unix {
    LIBS += -lgtest
}



QMAKE_CXXFLAGS += -Werror=constant-conversion


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




