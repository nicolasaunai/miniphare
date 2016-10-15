TEMPLATE=lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt



GTESTDIR = /Users/nicolasaunai/Downloads/googletest/googletest#$$(GTEST_ROOT)

# if macx else unix
macx {
    LIBS +=  -L$$GTESTDIR/lib -lgtest
    INCLUDEPATH += $$GTESTDIR/include $$GTESTDIR
} unix {
    #LIBS += -L/usr/lib -lpthread -lgtest -lgmock
    #INCLUDEPATH += /usr/include/gtest
    #INCLUDEPATH += /usr/include/gmock
}


SOURCES = $$GTESTDIR/src/gtest-all.cc

