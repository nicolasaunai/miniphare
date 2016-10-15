

GTESTINC    = $$PWD/../../googletest/googletest/include
GMOCKINC    = $$PWD/../../googletest/googlemock/include
GTESTDIRLIB = ./../GTest/GTest
GMOCKDIRLIB =./../GTest/GMock

LIBS +=  -L$$GTESTDIRLIB -lgtest  -L$$GMOCKDIRLIB -lgmock -pthread
INCLUDEPATH += $$GTESTINC $$GMOCKINC


check.target = check
check.commands = $$OUT_PWD/$$TARGET
check.depends = $$DESTDIR/$(QMAKE_TARGET)
