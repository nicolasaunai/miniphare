TEMPLATE=lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt


GTESTDIR = ../../googletest/googletest
GMOCKDIR = ../../googletest/googlemock


macx {
    LIBS        += -L$$GMOCKDIR/lib -lgmock
    INCLUDEPATH +=   $$GMOCKDIR/include $$GMOCKDIR $$GTESTDIR/include
} else {
 unix {
    #LIBS += -L/usr/lib -lpthread -lgtest -lgmock
    #INCLUDEPATH += /usr/include/gtest
    #INCLUDEPATH += /usr/include/gmock
}
}


SOURCES = $$GMOCKDIR/src/gmock-all.cc

