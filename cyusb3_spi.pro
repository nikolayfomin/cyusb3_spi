#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T13:46:44
#
#-------------------------------------------------

QT       -= gui

TARGET = cyusb3_spi
TEMPLATE = lib

DEFINES += CYUSB3_SPI_LIBRARY

SOURCES += cyusb3_spi.cpp

HEADERS += cyusb3_spi.h\
        cyusb3_spi_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
