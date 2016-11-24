TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_POST_LINK  = PYTHONPATH=$$PWD:$$PWD/.. $$PWD/test_pusher.py $$OUT_PWD


include(../GTest/GTest.pri)


INCLUDEPATH += ../../src



SOURCES += ../../src/Plasmas/particles.cpp \
           ../../src/pusher/modifiedboris.cpp \
           ../../src/pusher/pusher1d.cpp \
           ../../src/grid/gridlayout.cpp \
           ../../src/grid/gridlayoutimplyee.cpp \
           ../../src/grid/gridlayoutimplinternals.cpp \
           test_main.cpp \
           test_utilities.cpp \
           test_pusher1d.cpp

HEADERS += ../../src/Plasmas/particles.h \
           ../../src/pusher/pushertype.h \
           ../../src/pusher/modifiedboris.h \
           ../../src/pusher/pusher.h \
           ../../src/pusher/pusher1d.h \
           ../../src/pusher/pusherfactory.h \
           ../../src/grid/gridlayout.h \
           ../../src/grid/gridlayoutimplfactory.h \
           ../../src/grid/gridlayoutimplyee.h \
           ../../src/grid/gridlayoutimpl.h \
           ../../src/grid/gridlayoutimplinternals.h \
           ../../src/types.h \
           ../../src/constants.h \
           ../../src/utility.h \
           test_pusher.h

OTHER_FILES +=\
    test_pusher.py
