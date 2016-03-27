TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH+= \
    ions/ \


SOURCES += \
    pusher/pusher.cpp \
    ions/ions.cpp \
    ions/species.cpp \
    ions/particles.cpp \
    Field/field.cpp


SOURCES += main.cpp

HEADERS += \
    pusher/pusher.h \
    ions/ions.h \
    ions/species.h \
    ions/particles.h \
    Field/field.h

DISTFILES += \
    pusher/ModelPusher.qmodel \
    miniphare.qmodel


