TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH+= inc


SOURCES += \
    pusher/pusher.cpp


SOURCES += main.cpp

HEADERS += \
    pusher/pusher.h

DISTFILES += \
    pusher/ModelPusher.qmodel


