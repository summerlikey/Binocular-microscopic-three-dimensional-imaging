#-------------------------------------------------
#
# Project created by QtCreator 2019-04-19T14:04:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt3
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

CONFIG += c++11

INCLUDEPATH += $$(VIMBA_HOME)
INCLUDEPATH += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaCPP\include"
INCLUDEPATH += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaImageTransform\include"

LIBS += I:/qtcode/VimbaC/Lib/Win64/VimbaC.lib
LIBS += I:/qtcode/VimbaCPP/Lib/Win64/VimbaCPP.lib
LIBS += I:/qtcode/VimbaImageTransform/Lib/Win64/VimbaImageTransform.lib

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    vmbcamera.cpp \
    apicontrol.cpp

HEADERS += \
        mainwindow.h \
    vmbcamera.h \
    apicontrol.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
