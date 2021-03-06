#-------------------------------------------------
#
# Project created by QtCreator 2017-11-16T10:04:32
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = easyAuto
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    trans.cpp \
    form.cpp \
    strOp.cpp

HEADERS += \
        widget.h \
    trans.h \
    form.h \
    strOp.h

FORMS += \
        widget.ui \
    form.ui

INCLUDEPATH += d:\swh\e\eigen
INCLUDEPATH += D:\swh\e\C++\boost_1_65_1
QMAKE_CXXFLAGS += -std=c++11
CONFIG += mobility
CONFIG += c++11
MOBILITY = 

RESOURCES += \
    qrc1.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

android:{
DEFINES += FOR_MOBILE
}

