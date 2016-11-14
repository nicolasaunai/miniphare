TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_POST_LINK  = PYTHONPATH=$$PWD:$$PWD/.. $$PWD/test_IndexesAndWeights.py $$OUT_PWD


include(../GTest/GTest.pri)


INCLUDEPATH += ../../src



SOURCES += ../../src/grid/gridlayout.cpp \
           ../../src/grid/gridlayoutimplyee.cpp \
           ../../src/grid/gridlayoutimplinternals.cpp \
           ../../src/IndexesAndWeights/indexesandweights.cpp \
           ../../src/IndexesAndWeights/indexesandweightso1.cpp \
           ../../src/IndexesAndWeights/indexesandweightso2.cpp \
           ../../src/IndexesAndWeights/indexesandweightso3.cpp \
           ../../src/IndexesAndWeights/indexesandweightso4.cpp \
           test_main.cpp \
           test_utilities.cpp \
    test_weights.cpp \
    test_indexes.cpp

HEADERS += ../../src/types.h \
           ../../src/constants.h \
           ../../src/utility.h \
           ../../src/grid/gridlayout.h \
           ../../src/grid/gridlayoutimplfactory.h \
           ../../src/grid/gridlayoutimplyee.h \
           ../../src/grid/gridlayoutimpl.h \
           ../../src/grid/gridlayoutimplinternals.h \
           ../../src/IndexesAndWeights/indexesandweights.h \
           ../../src/IndexesAndWeights/indexesandweightso1.h \
           ../../src/IndexesAndWeights/indexesandweightso2.h \
           ../../src/IndexesAndWeights/indexesandweightso3.h \
           ../../src/IndexesAndWeights/indexesandweightso4.h \
    test_indexes.h \
    test_weights.h

OTHER_FILES +=\
    test_IndexesAndWeights.py


