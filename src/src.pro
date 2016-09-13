TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    utility.cpp \
    AMR/hierarchy.cpp \
    AMR/patch.cpp \
    AMR/patchdata.cpp \
    AMR/mlmd.cpp \
    Electromag/electromag.cpp \
    Faraday/faraday.cpp \
    Faraday/faradayimpl1d.cpp \
    Field/field.cpp \
    grid/gridlayout.cpp \
    grid/gridlayoutimpl.cpp \
    grid/gridlayoutimplyee.cpp \
    Ohm/ohm.cpp \
    Ohm/ohmimpl1d.cpp \
    Plasmas/electrons.cpp \
    Plasmas/ions.cpp \
    Plasmas/particles.cpp \
    Plasmas/species.cpp \
    pusher/pusher.cpp \
    Solver/solver.cpp \
    vecfield/vecfield.cpp

HEADERS += \
    constants.h \
    types.h \
    utility.h \
    AMR/patch.h \
    AMR/patchdata.h \
    AMR/hierarchy.h \
    AMR/mlmd.h \
    Electromag/electromag.h \
    Faraday/faraday.h \
    Faraday/faradayimpl.h \
    Faraday/faradayimpl1d.h \
    Faraday/faradayfactory.h \
    Field/field.h \
    grid/gridlayout.h \
    grid/gridlayoutimpl.h \
    grid/gridlayoutimplyee.h \
    grid/gridlayoutimplfactory.h \
    Ohm/ohm.h \
    Ohm/ohmimplfactory.h \
    Ohm/ohmimpl1d.h \
    Plasmas/electrons.h \
    Plasmas/ions.h \
    Plasmas/particles.h \
    Plasmas/species.h \
    pusher/pusher.h \
    Solver/solver.h \
    vecfield/vecfield.h \
    hybridenums.h


DISTFILES += \
    pusher/ModelPusher.qmodel
