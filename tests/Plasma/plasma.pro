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
           ../../src/Plasmas/ions.cpp \


HEADERS += ../../src/Plasmas/ions.h \




