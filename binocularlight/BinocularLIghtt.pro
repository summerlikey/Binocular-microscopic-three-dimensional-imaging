#-------------------------------------------------
#
# Project created by QtCreator 2013-03-01T11:12:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BinocularLightGui
TEMPLATE = app


SOURCES +=  main.cpp\
            apicontroller.cpp \
            cameramainwindow.cpp \
            cameraobserver.cpp \
            dlpc350_common.cpp \
            dlpc350_usb.cpp \
            dlpc350_api.cpp \
            dlpc350_BMPParser.cpp \
            dlpc350_firmware.cpp \
            frameobserver.cpp \
            lightmainwindow.cpp

HEADERS  += \
            apicontroller.h \
            cameramainwindow.h \
            cameraobserver.h \
            dlpc350_usb.h \
            dlpc350_api.h \
            dlpc350_BMPParser.h \
            dlpc350_firmware.h \
            dlpc350_version.h \
            dlpc350_common.h \
            dlpc350_flashDevice.h \
            Helper.h \
            dlpc350_error.h \
            errorcodetomessage.h \
            frameobserver.h \
            lightmainwindow.h

INCLUDEPATH += "hidapi-master\\hidapi"

FORMS    += \
    cameramainwindow.ui \
    lightmainwindow.ui

RESOURCES += Lcr2.qrc \
    icon.qrc

# Icons
macx{
ICON = icons/LCr4500.icns
}

win32{
RC_FILE = icons\LCr4500.rc
}

#linux
INCLUDEPATH += /home/ganggang/Downloads/Vimba_2_1/
INCLUDEPATH += /home/ganggang/Downloads/Vimba_2_1/VimbaImageTransform/Include
LIBS += -L"/home/ganggang/Downloads/Vimba_2_1/VimbaCPP/DynamicLib/x86_64bit/"
LIBS += -L"/home/ganggang/Downloads/Vimba_2_1/VimbaImageTransform/DynamicLib/x86_64bit/"
LIBS += -lVimbaC -lVimbaCPP -lVimbaImageTransform

win32:CONFIG(release, debug|release): LIBS += -L../binocularlight/hidapi-master/windows/release/ -lhidapi
else:win32:CONFIG(debug, debug|release): LIBS += -L../binocularlight/hidapi-master/windows/debug/ -lhidapi
macx: SOURCES += ../binocularlight/hidapi-master/mac/hid.c
unix: !macx: SOURCES += ../binocularlight/hidapi-master/linux/hid.c

macx: LIBS += -framework CoreFoundation -framework IOkit
win32: LIBS += -lSetupAPI
unix: !macx: LIBS += -lusb-1.0 -ludev

INCLUDEPATH += ../binocularlight/hidapi-master/hidapi
DEPENDPATH += ../binocularlight/hidapi-master/hidapi

OTHER_FILES +=  icons\LCr4500.rc \
                icons\LCr4500.ico
