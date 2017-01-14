cmake_minimum_required (VERSION 2.6)
project (miniphare)

find_package(MPI)



set(SOURCES main.cpp
    utility.cpp
    Plasmas/ions.cpp
    Plasmas/particles.cpp
    Plasmas/species.cpp
    Plasmas/electrons.cpp
    pusher/pusher1d.cpp
    pusher/modifiedboris.cpp
    Solver/solver.cpp
    Electromag/electromag.cpp
    vecfield/vecfield.cpp
    Field/field.cpp
    AMR/hierarchy.cpp
    AMR/patch.cpp
    AMR/patchdata.cpp
    AMR/mlmd.cpp
    Faraday/faraday.cpp
    Faraday/faradayimpl1d.cpp
    grid/gridlayout.cpp
    grid/gridlayoutimplyee.cpp
    grid/gridlayoutimplinternals.cpp
    Ohm/ohm.cpp
    Ohm/ohmimpl1d.cpp
    Initializer/ionsinitializer.cpp
    Initializer/electromaginitializer.cpp
    Initializer/ohminitializer.cpp
    Initializer/pythoninitializerfactory.cpp
    Initializer/particleinitializer.cpp
    Initializer/simpleinitializerfactory.cpp
    Initializer/initializerfactory.cpp
    Initializer/fluidparticleinitializer.cpp
    helper.cpp
    IndexesAndWeights/indexesandweights.cpp
    IndexesAndWeights/indexesandweightso1.cpp
    IndexesAndWeights/indexesandweightso2.cpp
    IndexesAndWeights/indexesandweightso3.cpp
    IndexesAndWeights/indexesandweightso4.cpp
    Interpolator/interpolator.cpp
    Projector/projector.cpp
)

add_executable(miniphare ${SOURCES})
