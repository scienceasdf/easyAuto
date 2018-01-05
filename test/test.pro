TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    transTest.cpp

INCLUDEPATH += d:\swh\e\eigen

SOURCES += \
    ..\trans.cpp \

HEADERS += \
    ..\trans.h \
    util.h \
    transTest.h
