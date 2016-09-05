TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SUBDIRS += gridlayout


INCLUDEPATH += /opt/local/include \
               ../../src

#LIBS += -L/opt/local/lib -lgtest

# if macx else unix
macx {
    LIBS += -L/opt/local/lib -lgtest
} unix {
    LIBS += -lgtest
}




QMAKE_CXXFLAGS += -Werror=constant-conversion


SOURCES += test_gridlayout.cpp \
           ../../src/grid/gridlayout.cpp \
           ../../src/grid/gridlayoutimplyee.cpp \
           ../../src/grid/gridlayoutimpl.cpp \

HEADERS += ../../src/grid/gridlayout.h \
           ../../src/grid/gridlayoutimplfactory.h \
           ../../src/grid/gridlayoutimplyee.h \
           ../../src/grid/gridlayoutimpl.h \
           ../../src/types.h \
           ../../src/constants.h \
           ../../src/utility.h \
