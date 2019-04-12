#-------------------------------------------------
#
# Project created by QtCreator 2018-12-22T12:01:26
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = reamber
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += D:\qtdoc\reamber_base\src
LIBS += -LD:\qtdoc\build-reamber_base-Desktop_Qt_5_12_0_MSVC2017_32bit-Release\release -lreamber_base

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-reamber_base-Desktop_Qt_5_12_0_MSVC2017_32bit-Release/release/ -lreamber_base
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-reamber_base-Desktop_Qt_5_12_0_MSVC2017_32bit-Release/debug/ -lreamber_base
else:unix: LIBS += -L$$PWD/../build-reamber_base-Desktop_Qt_5_12_0_MSVC2017_32bit-Release/ -lreamber_base

INCLUDEPATH += $$PWD/../build-reamber_base-Desktop_Qt_5_12_0_MSVC2017_32bit-Release/release
DEPENDPATH += $$PWD/../build-reamber_base-Desktop_Qt_5_12_0_MSVC2017_32bit-Release/release

SOURCES += \
    src/main/main.cpp \
    src/main/mainwindow.cpp \
    src/dependencies/qcustomplot.cpp
HEADERS += \
    src/main/mainwindow.h \
    src/dependencies/qcustomplot.h

FORMS += \
    src/ui/mainwindow.ui

DISTFILES += \
    naming_conventions \
    src/README.md \
    src/naming_conventions

RESOURCES += \
    rsc.qrc

RC_ICONS = rsc/amber.ico





