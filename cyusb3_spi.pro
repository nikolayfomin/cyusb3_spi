#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T13:46:44
#
#-------------------------------------------------

QT       -= gui

TARGET = cyusb3_spi
TEMPLATE = lib

DEFINES += CYUSB3_SPI_LIBRARY

LIBS += ..\source\library\lib\x86\CyAPI.lib
LIBS += /NODEFAULTLIB:LIBCMT
LIBS += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\User32.lib"
LIBS += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\setupapi.lib"

SOURCES += cyusb3_spi.cpp

HEADERS += cyusb3_spi.h\
        cyusb3_spi_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
