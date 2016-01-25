#ifndef CYUSB3_SPI_GLOBAL_H
#define CYUSB3_SPI_GLOBAL_H

#include <QtCore/qglobal.h>

extern "C" Q_DECL_EXPORT bool   cy3_OpenDevice(void);
extern "C" Q_DECL_EXPORT void   cy3_CloseDevice(void);
extern "C" Q_DECL_EXPORT bool   cy3_LoadRAM(const char* FileName);
extern "C" Q_DECL_EXPORT int    cy3_WriteSPI(const unsigned char addr, const unsigned char data);
extern "C" Q_DECL_EXPORT int    cy3_ReadSPI(const unsigned char addr, unsigned char* data);

#endif // CYUSB3_SPI_GLOBAL_H
