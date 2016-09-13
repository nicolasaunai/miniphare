TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Plasmas/ions.cpp \
    Plasmas/particles.cpp \
    Plasmas/species.cpp \
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
    grid/gridlayoutimpl.cpp \
    utility.cpp \
    Plasmas/particleloader.cpp \
    Initializer/ionsinitializer.cpp \
    Initializer/electromaginitializer.cpp \
    Initializer/ohminitializer.cpp \
    Initializer/pythoninitializerfactory.cpp \
    Initializer/particleinitializer.cpp \
    Initializer/fluidparticleinitializer.cpp


HEADERS += \
    Plasmas/ions.h \
    Plasmas/particles.h \
    Plasmas/species.h \
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
    constants.h \
    utility.h \
    Plasmas/particleloader.h \
    Initializer/initializerfactory.h \
    Initializer/ionsinitializer.h \
    Initializer/electromaginitializer.h \
    Initializer/ohminitializer.h \
    Initializer/pythoninitializerfactory.h \
    Initializer/particleinitializer.h \
    Initializer/fluidparticleinitializer.h



DISTFILES += \
    pusher/ModelPusher.qmodel \
    Plasmas/Plasma.qmodel
