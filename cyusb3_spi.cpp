#include "cyusb3_spi.h"

//////// EXPORT LAYER ////////
bool cy3_OpenDevice(void)
{
    DTParams.USBDevice = new CCyFX3Device;

    if ( ReviewDevices() )
        return false;

    //if ( LoadRAM("..\\SlaveFifoSync.img") ) {
    //if ( LoadRAM("..\\fx3_newsub_injector.img") ) {
    //    return false;
    //}

    if (GetStreamerDevice())
    {
        delete DTParams.USBDevice;
        return false;
    }


    return true;
}

void cy3_CloseDevice(void)
{
    delete DTParams.USBDevice;
}

bool cy3_LoadRAM(const char* FileName)
{
    if (LoadRAM(FileName))
        return false;

    return true;
}

int cy3_WriteSPI(const unsigned char addr, const unsigned char data)
{
    return Send16bitSPI(addr,data);
}

int cy3_ReadSPI(const unsigned char addr, unsigned char* data)
{
    return Read16bitSPI(addr,data);
}

//////// INTERNAL FUNCTIONALITY ////////

int ReviewDevices() {
    if (DTParams.USBDevice == NULL) {
        //qFatal( "Error: ReviewDevices::GetStreamerDevice() USBDevice == NULL" );
        return -1;
    }
    int dev_cnt = DTParams.USBDevice->DeviceCount();
    //qDebug( "ReviewDevices(): found %d devices", dev_cnt );

    int suitable_dev_cnt = 0;
    for (int i=0; i<dev_cnt; i++)
    {
        unsigned short product = DTParams.USBDevice->ProductID;
        unsigned short vendor  = DTParams.USBDevice->VendorID;
        bool suitable = ( vendor == VENDOR_ID ) && ( (product == PRODUCT_ID) || (product == PRODUCT_ID2) );
        //qDebug( "Device[%2d]: 0x%04X 0x%04X %s", i, vendor, product, suitable ? "***" : "" );
        if ( suitable ) {
            suitable_dev_cnt++;
        }
    }
    if (suitable_dev_cnt == 0) {
        return -2;
    } else {
        return 0;
    }
}

int LoadRAM(const char* fwFileName)
{
    //qDebug() << "LoadRAM( '" << fwFileName << "' )";
    /*QFile cf(fwFileName);
    if (!cf.open(QFile::ReadOnly)) {
        qFatal( "LoadRAM file '%s' io error!", fwFileName );
        return -1;
    } else {
        cf.close();
    }*/

    FILE *f = fopen(fwFileName, "rt");

    if (!f)
        return -1;
    else
        fclose(f);

    if (NULL != DTParams.USBDevice) {
        //qDebug() << "StartParams.USBDevice->Open(0)" << StartParams.USBDevice->Open(0);
        if (DTParams.USBDevice->IsBootLoaderRunning())
        {
            int retCode  = DTParams.USBDevice->DownloadFw((char*)fwFileName, FX3_FWDWNLOAD_MEDIA_TYPE::RAM);
            if (FX3_FWDWNLOAD_ERROR_CODE::SUCCESS == retCode)
            {
                Sleep(1000);
                return retCode;
            }
            else return -1;
        }
    } else {
        //qDebug() << "LoadRAM(): USBDevice is NULL";
        return -2;
    }
    return 0;
}

int GetStreamerDevice(){

    //qDebug() << "CyThread::GetStreamerDevice()";
    //StartParams.USBDevice = new CCyFX3Device;//(NULL, CYUSBDRV_GUID,true);

    if (DTParams.USBDevice == NULL) {
        //qDebug() << "Error: CyThread::GetStreamerDevice() USBDevice == NULL";
        return -1;
    }

    int n = DTParams.USBDevice->DeviceCount();

    // Walk through all devices looking for VENDOR_ID/PRODUCT_ID
    for (int i=0; i<n; i++)
    {
        /*qDebug( "Device[%2d]: 0x%04X 0x%04X",
                i,
                DTParams.USBDevice->VendorID,
                DTParams.USBDevice->ProductID );*/
        if ((DTParams.USBDevice->VendorID == VENDOR_ID) && (DTParams.USBDevice->ProductID == PRODUCT_ID)) {
            //qDebug( "Matched!" );
            break;
        }

        DTParams.USBDevice->Open(i);
    }

    if ((DTParams.USBDevice->VendorID == VENDOR_ID) && (DTParams.USBDevice->ProductID == PRODUCT_ID))
    {
        int interfaces = DTParams.USBDevice->AltIntfcCount()+1;

        DTParams.bHighSpeedDevice = DTParams.USBDevice->bHighSpeed;
        DTParams.bSuperSpeedDevice = DTParams.USBDevice->bSuperSpeed;

        for (int i=0; i< interfaces; i++)
        {
            DTParams.USBDevice->SetAltIntfc(i);

            int eptCnt = DTParams.USBDevice->EndPointCount();

            // Fill the EndPointsBox
            for (int e=1; e<eptCnt; e++)
            {
                CCyUSBEndPoint *ept = DTParams.USBDevice->EndPoints[e];
                // INTR, BULK and ISO endpoints are supported.
                if ((ept->Attributes >= 1) && (ept->Attributes <= 3))
                {
                    EndPointParams Params;
                    Params.Attr = ept->Attributes;
                    Params.In = ept->bIn;
                    Params.MaxPktSize = ept->MaxPktSize;
                    Params.MaxBurst = DTParams.USBDevice->BcdUSB == 0x0300 ? ept->ssmaxburst : 0;
                    Params.Interface = i;
                    Params.Address = ept->Address;

                    EndPointsParams.push_back(Params);
                }
            }
        }
    }

    return 0;
}

int Read16bitSPI(unsigned char addr, unsigned char* data)
{
    UCHAR buf[16];
    //addr |= 0x8000;
    buf[0] = (UCHAR)(*data);
    buf[1] = (UCHAR)(addr|0x80);

    CCyControlEndPoint* CtrlEndPt;
    CtrlEndPt = DTParams.USBDevice->ControlEndPt;
    CtrlEndPt->Target = TGT_DEVICE;
    CtrlEndPt->ReqType = REQ_VENDOR;
    CtrlEndPt->Direction = DIR_FROM_DEVICE;
    CtrlEndPt->ReqCode = 0xB5;
    CtrlEndPt->Value = *data;
    CtrlEndPt->Index = addr|0x80;
    long len = 16;
    int success = CtrlEndPt->XferData(&buf[0], len);

    *data = buf[0];
    //qDebug() << "Reg" << (addr) << " " << hex << *data;

    return success;
}

int Send16bitSPI(unsigned char addr, unsigned char data)
{
    UCHAR buf[16];
    buf[0] = (UCHAR)(data);
    buf[1] = (UCHAR)(addr);

    //qDebug() << "Reg" << addr << " " << hex << data;

    CCyControlEndPoint* CtrlEndPt;
    CtrlEndPt = DTParams.USBDevice->ControlEndPt;
    CtrlEndPt->Target = TGT_DEVICE;
    CtrlEndPt->ReqType = REQ_VENDOR;
    CtrlEndPt->Direction = DIR_TO_DEVICE;
    CtrlEndPt->ReqCode = 0xB3;
    CtrlEndPt->Value = 0;
    CtrlEndPt->Index = 1;
    long len = 16;
    int success = CtrlEndPt->XferData(&buf[0], len);

    return success;
}
