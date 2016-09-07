TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SUBDIRS += plasma


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
           ../../src/grid/gridLayoutimpl.cpp \
           ../../src/grid/gridLayoutimplyee.cpp \
           ../../src/Plasmas/species.cpp \



HEADERS += ../../src/Plasmas/ions.h \
           ../../src/grid/gridLayout.h \
           ../../src/grid/gridLayoutimpl.h \
           ../../src/grid/gridLayoutimplyee.h \
           ../../src/Plasmas/species.h \
           ../../src/Field/field.h \




