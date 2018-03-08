# PawdFox

A Firefox Saved Accounts Viewer using QT

## Build Instructions

 1. Install QT Creator with QT sources
 2. Build NSS
 3. Build libpawdfox (https://github.com/psychart/libpawdfox)
 4. Modify QT-PawdFox.pro to set libpawdfox, nss and rapidjson paths
 5. Build 

### Static build

For static builds is necessary to build QT5.

    set PATH=C:\Qt\Tools\mingw530_32\bin\;%PATH%;C:\Apps\Python27;C:\qt\qt-everywhere-src-5.10.0\gnuwin32\bin
    configure -opensource -confirm-license  -release -static -platform win32-g++ -opengl desktop -prefix C:\qt-5.10-static\ -qt-zlib -qt-libjpeg -qt-libpng -nomake tests -nomake examples