TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_POST_LINK  = PYTHONPATH=$$PWD:$$PWD/.. $$PWD/test_pusher.py $$OUT_PWD


include(../GTest/GTest.pri)


INCLUDEPATH += ../../src


SOURCES += ../../src/Plasmas/particles.cpp \
           ../../src/Plasmas/species.cpp \
           ../../src/Projector/projector.cpp \
           ../../src/Interpolator/interpolator.cpp \
           ../../src/IndexesAndWeights/indexesandweights.cpp \
           ../../src/IndexesAndWeights/indexesandweightso1.cpp \
           ../../src/IndexesAndWeights/indexesandweightso2.cpp \
           ../../src/IndexesAndWeights/indexesandweightso3.cpp \
           ../../src/IndexesAndWeights/indexesandweightso4.cpp \
           ../../src/Initializer/particleinitializer.cpp \
           ../../src/Initializer/simpleparticleinitializer.cpp \
           ../../src/pusher/modifiedboris.cpp \
           ../../src/pusher/pusher1d.cpp \
           ../../src/grid/gridlayout.cpp \
           ../../src/grid/gridlayoutimplyee.cpp \
           ../../src/grid/gridlayoutimplinternals.cpp \
           ../../src/vecfield/vecfield.cpp \
           ../../src/Field/field.cpp \
           test_main.cpp \
           test_utilities.cpp \
           test_commons.cpp \
           test_pusher1d.cpp \
           test_interpolate1d.cpp


HEADERS += ../../src/Plasmas/particles.h \
           ../../src/Plasmas/species.h \
           ../../src/Projector/projector.h \
           ../../src/Interpolator/interpolatorfactory.h \
           ../../src/Interpolator/interpolator.h \
           ../../src/IndexesAndWeights/indexesandweights.h \
           ../../src/IndexesAndWeights/indexesandweightso1.h \
           ../../src/IndexesAndWeights/indexesandweightso2.h \
           ../../src/IndexesAndWeights/indexesandweightso3.h \
           ../../src/IndexesAndWeights/indexesandweightso4.h \
           ../../src/Initializer/particleinitializer.h \
           ../../src/Initializer/simpleparticleinitializer.h \
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
           ../../src/vecfield/vecfield.h \
           ../../src/Field/field.h \
           ../../src/types.h \
           ../../src/constants.h \
           ../../src/utility.h \
           test_commons.h \
           test_pusher.h \
           test_interpolate1d.h

OTHER_FILES +=\
    test_pusher.py
