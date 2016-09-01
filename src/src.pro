TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Plasmas/ions.cpp \
    Plasmas/particles.cpp \
    Plasmas/species.cpp \
    Field/field.cpp \
    pusher/pusher.cpp \
    Solver/solver.cpp \
    Electromag/electromag.cpp \
    vecfield/vecfield.cpp \
    Field/field.cpp \
    Faraday/faraday.cpp \
    Plasmas/electrons.cpp \
    AMR/patch.cpp \
    AMR/patchdata.cpp \
    AMR/hierarchy.cpp \
    grid/gridlayout.cpp \
    AMR/mlmd.cpp \
    grid/gridlayoutimplyee.cpp \
    Ohm/ohm.cpp \
    Ohm/ohmimpl1d.cpp \
    Faraday/faradayimpl1d.cpp \
    grid/gridlayoutimpl.cpp

HEADERS += \
    Plasmas/ions.h \
    Plasmas/particles.h \
    Plasmas/species.h \
    Field/field.h \
    pusher/pusher.h \
    Solver/solver.h \
    Electromag/electromag.h \
    types.h \
    vecfield/vecfield.h \
    Field/field.h \
    Faraday/faraday.h \
    Faraday/faradayfactory.h \
    Plasmas/electrons.h \
    AMR/patch.h \
    AMR/patchdata.h \
    AMR/hierarchy.h \
    grid/gridlayout.h \
    AMR/mlmd.h \
    grid/gridlayoutimplyee.h \
    grid/gridlayoutimplfactory.h \
    Ohm/ohm.h \
    Ohm/ohmimplfactory.h \
    Ohm/ohmimpl1d.h \
    Faraday/faradayimpl1d.h \
    grid/gridlayoutimpl.h \
    constants.h



DISTFILES += \
    pusher/ModelPusher.qmodel
