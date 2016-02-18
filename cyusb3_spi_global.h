#ifndef CYUSB3_SPI_GLOBAL_H
#define CYUSB3_SPI_GLOBAL_H

#include <QtCore/qglobal.h>

extern "C" Q_DECL_EXPORT bool   cy3_OpenDevice(const char* FileName);
extern "C" Q_DECL_EXPORT void   cy3_CloseDevice(void);
// returns 1 on successful RAM load, 0 if RAM already loaded, <0 on error
//extern "C" Q_DECL_EXPORT int    cy3_LoadRAM(const char* FileName);
extern "C" Q_DECL_EXPORT int    cy3_WriteSPI(const unsigned char addr, const unsigned char data);
extern "C" Q_DECL_EXPORT int    cy3_ReadSPI(const unsigned char addr, unsigned char* data);

#endif // CYUSB3_SPI_GLOBAL_H
