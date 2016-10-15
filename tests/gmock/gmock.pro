TEMPLATE=lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt


GTESTDIR = /Users/nicolasaunai/Downloads/googletest/googletest#$$(GTEST_ROOT)
GMOCKDIR = /Users/nicolasaunai/Downloads/googletest/googlemock#$$(GMOCK_ROOT)

# if macx else unix
macx {
    LIBS +=  -L$$GMOCKDIR/lib -lgmock
    INCLUDEPATH += $$GMOCKDIR/include $$GMOCKDIR $$GTESTDIR/include
} unix {
    #LIBS += -L/usr/lib -lpthread -lgtest -lgmock
    #INCLUDEPATH += /usr/include/gtest
    #INCLUDEPATH += /usr/include/gmock
}


SOURCES = $$GMOCKDIR/src/gmock-all.cc

