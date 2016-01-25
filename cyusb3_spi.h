#ifndef CYUSB3_SPI_H
#define CYUSB3_SPI_H

#include "cyusb3_spi_global.h"
#include <windows.h>
#include <stdio.h>
#include "library\inc\CyAPI.h"
#include <vector>

#define MAX_QUEUE_SZ  64
#define VENDOR_ID    ( 0x04B4 )
#define PRODUCT_ID   ( 0x00F1 )
#define PRODUCT_ID2  ( 0x00F3 )

typedef void (__stdcall * DataProcessorFunc)(char*, int);//data pointer, data size (bytes);

struct EndPointParams{
    int Attr;
    bool In;
    int MaxPktSize;
    int MaxBurst;
    int Interface;
    int Address;
};

struct DataTransferParams{
    CCyFX3Device	*USBDevice;
    CCyUSBEndPoint  *EndPt;
    int				PPX;
    int				QueueSize;
    int				TimeOut;
    bool			bHighSpeedDevice;
    bool			bSuperSpeedDevice;
    bool			bStreaming;
    bool			ThreadAlreadyStopped;
    DataProcessorFunc DataProc;
    EndPointParams  CurEndPoint;
};

DataTransferParams DTParams;
std::vector<EndPointParams> EndPointsParams;

int ReviewDevices();
int LoadRAM(const char* fwFileName);
int GetStreamerDevice();
int Read16bitSPI(unsigned char addr, unsigned char* data);
int Send16bitSPI(unsigned char addr, unsigned char data);

#endif // CYUSB3_SPI_H
