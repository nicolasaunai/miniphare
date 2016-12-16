TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += main.cpp \
    utility.cpp \
    Plasmas/ions.cpp \
    Plasmas/particles.cpp \
    Plasmas/species.cpp \
    Plasmas/electrons.cpp \
    Solver/solver.cpp \
    Electromag/electromag.cpp \
    vecfield/vecfield.cpp \
    Field/field.cpp \
    AMR/hierarchy.cpp \
    AMR/patch.cpp \
    AMR/patchdata.cpp \
    AMR/mlmd.cpp \
    Faraday/faraday.cpp \
    Faraday/faradayimpl1d.cpp \
    grid/gridlayout.cpp \
    grid/gridlayoutimplyee.cpp \
    grid/gridlayoutimplinternals.cpp \
    Ohm/ohm.cpp \
    Ohm/ohmimpl1d.cpp \
    Initializer/ionsinitializer.cpp \
    Initializer/electromaginitializer.cpp \
    Initializer/ohminitializer.cpp \
    Initializer/pythoninitializerfactory.cpp \
    Initializer/particleinitializer.cpp \
    Initializer/simpleinitializerfactory.cpp \
    Initializer/initializerfactory.cpp \
    Initializer/fluidparticleinitializer.cpp \
    IndexesAndWeights/indexesandweights.cpp \
    IndexesAndWeights/indexesandweightso1.cpp \
    IndexesAndWeights/indexesandweightso2.cpp \
    IndexesAndWeights/indexesandweightso3.cpp \
    IndexesAndWeights/indexesandweightso4.cpp \
    Interpolator/interpolator.cpp \
    pusher/pusher1d.cpp \
    pusher/modifiedboris.cpp \
    Projector/projector.cpp \
    helper.cpp


HEADERS += \
    Plasmas/ions.h \
    Plasmas/particles.h \
    Plasmas/species.h \
    Plasmas/electrons.h \
    Solver/solver.h \
    Electromag/electromag.h \
    vecfield/vecfield.h \
    constants.h \
    types.h \
    utility.h \
    hybridenums.h \
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
    grid/gridlayoutimplinternals.h \
    grid/gridlayoutdefs.h \
    Ohm/ohm.h \
    Ohm/ohmimplfactory.h \
    Ohm/ohmimpl1d.h \
    Initializer/initializerfactory.h \
    Initializer/ionsinitializer.h \
    Initializer/electromaginitializer.h \
    Initializer/ohminitializer.h \
    Initializer/pythoninitializerfactory.h \
    Initializer/particleinitializer.h \
    Initializer/simpleinitializerfactory.h \
    Initializer/fluidparticleinitializer.h \
    IndexesAndWeights/indexesandweights.h \
    IndexesAndWeights/indexesandweightso1.h \
    IndexesAndWeights/indexesandweightso2.h \
    IndexesAndWeights/indexesandweightso3.h \
    IndexesAndWeights/indexesandweightso4.h \
    Interpolator/interpolator.h \
    Projector/projector.h \
    pusher/pusher.h \
    pusher/pusher1d.h \
    pusher/pushertype.h \
    pusher/pusherfactory.h \
    pusher/modifiedboris.h \
    helper.h \
    Initializer/solverinitializer.h



DISTFILES += \
    Plasmas/Plasma.qmodel\
    Initializer/Initializer.qmodel\
    pusher/pusher.qmodel
