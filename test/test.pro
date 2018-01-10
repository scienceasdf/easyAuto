TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    transTest.cpp \
    strOpTest.cpp \
    comprehensiveTest.cpp

INCLUDEPATH += d:\swh\e\eigen
INCLUDEPATH += D:\swh\e\C++\boost_1_65_1

SOURCES += \
    ..\trans.cpp \
    ..\strOp.cpp

HEADERS += \
    ..\trans.h \
    util.h \
    transTest.h \
    strOpTest.h \
    ..\strOp.h \
    comprehensiveTest.h
