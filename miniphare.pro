TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/Plasmas/ions.cpp \
    src/Plasmas/particles.cpp \
    src/Plasmas/species.cpp \
    src/Field/field.cpp \
    src/pusher/pusher.cpp \
    src/Solver/solver.cpp \
    src/Electromag/electromag.cpp \
    src/vecfield/vecfield.cpp \
    src/Field/field.cpp \
    src/Faraday/faraday.cpp \
    src/Plasmas/electrons.cpp \
    src/AMR/patch.cpp \
    src/AMR/patchdata.cpp \
    src/AMR/hierarchy.cpp \
    src/grid/gridlayout.cpp \
    src/AMR/mlmd.cpp \
    src/grid/gridlayoutimplyee.cpp \
    src/Ohm/ohm.cpp \
    src/Ohm/ohmimpl1d.cpp

HEADERS += \
    src/Plasmas/ions.h \
    src/Plasmas/particles.h \
    src/Plasmas/species.h \
    src/Field/field.h \
    src/pusher/pusher.h \
    src/Solver/solver.h \
    src/Electromag/electromag.h \
    src/types.h \
    src/vecfield/vecfield.h \
    src/Field/field.h \
    src/Faraday/faraday.h \
    src/Faraday/faradayfactory.h \
    src/Plasmas/electrons.h \
    src/AMR/patch.h \
    src/AMR/patchdata.h \
    src/AMR/hierarchy.h \
    src/grid/gridlayout.h \
    src/AMR/mlmd.h \
    src/grid/gridlayoutimplyee.h \
    src/grid/gridlayoutimplfactory.h \
    src/Ohm/ohm.h \
    src/Ohm/ohmimplfactory.h \
    src/Ohm/ohmimpl1d.h


INCLUDEPATH += src/


DISTFILES += \
    src/pusher/ModelPusher.qmodel
