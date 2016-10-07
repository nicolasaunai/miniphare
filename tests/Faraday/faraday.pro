TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SUBDIRS += faraday


# if macx else unix
macx {
    LIBS += -L/opt/local/lib -lgtest -lgmock
    INCLUDEPATH += /opt/local/include
} unix {
    LIBS += -L/usr/lib -lpthread -lgtest -lgmock
    INCLUDEPATH += /usr/include/gtest
    INCLUDEPATH += /usr/include/gmock
}

INCLUDEPATH += ../../src


#    QMAKE_CXXFLAGS +=  -Werror=constant-conversion



SOURCES += ../../src/Faraday/faraday.cpp \
           ../../src/Faraday/faradayimpl1d.cpp \
           ../../src/grid/gridlayout.cpp \
           ../../src/grid/gridlayoutimplyee.cpp \
           ../../src/grid/gridlayoutimpl.cpp \
           ../../src/Field/field.cpp \ 
           ../../src/vecfield/vecfield.cpp \
           test_main.cpp \
           test_utilities.cpp \
           test_faraday1d.cpp 

HEADERS += ../../src/Faraday/faradayfactory.h \
           ../../src/Faraday/faraday.h \
           ../../src/Faraday/faradayimpl.h \
           ../../src/Faraday/faradayimpl1d.h \
           ../../src/grid/gridlayout.h \
           ../../src/grid/gridlayoutimplfactory.h \
           ../../src/grid/gridlayoutimplyee.h \
           ../../src/grid/gridlayoutimpl.h \
           ../../src/types.h \
           ../../src/constants.h \
           ../../src/utility.h \
           ../../src/Field/field.h \ 
           ../../src/vecfield/vecfield.h \
           test_faraday.h
