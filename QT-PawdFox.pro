#-------------------------------------------------
#
# Project created by QtCreator 2018-01-18T14:36:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PawdFox
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

ICON = if_keyhole_1930259.icns
RC_FILE = myapp.rc

CONFIG += no_keywords

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        aboutdialog.cpp \
    revealpassworddialog.cpp

HEADERS += \
        mainwindow.h \
        aboutdialog.h \
    revealpassworddialog.h

#DUMBLIB = 1
#DEFINES += DUMBLIB

count(DUMBLIB, 0) {
    message(Using libpawdfox)
    macx {
        QMAKE_TARGET_BUNDLE_PREFIX = com.gmail.sdepaula.foxpwd
        INCLUDEPATH += "/usr/local/opt/nss/include" \
                           "/usr/local/opt/nspr/include/nspr" \
                           "/usr/local/Cellar/rapidjson/1.1.0/include/" \
                           "../../libpawdfox/release/usr/local/include/"

        unix:QMAKE_LFLAGS += "-L/usr/local/opt/nss/lib" \
                            "-L/usr/local/opt/nspr/lib/"
    }
    unix:INCLUDEPATH += "/usr/include/nspr"

    win32:INCLUDEPATH += "..\rapidjson\include" \
                            "..\libnss\nss-3.33\dist\WIN954.0_DBG.OBJ/include" \
                            "..\libnss\nss-3.33\dist\public"

    win32:LIBS += "-L..\libnss\nss-3.33\dist\WIN954.0_DBG.OBJ\lib"

    LIBS += -lnss3 ../../libpawdfox/release/usr/local/lib/libpawdfox.a
}

count(DUMBLIB, 1) {
    message(Using DUMBLIB)
    SOURCES += temp.cpp
    HEADERS += temp.h
}



FORMS += \
        mainwindow.ui \
        aboutdialog.ui \
    revealpassworddialog.ui

DISTFILES += \
    if_keyhole_1930259.ico \
    myapp.rc \
    if_keyhole_1930259.png

RESOURCES += \
    myapp.qrc

