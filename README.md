# Dynamic library for NT1065 Cypress USB3 board

External functions:
- bool   cy3_OpenDevice(const char* FileName)

  Open device interface and flash firmware FileName if needed. Device needs to be opened prior to using WriteSPI/ReadSPI.
  Returns TRUE if device is opened successfully.
  
- void   cy3_CloseDevice(void)

  Close device after it was opened.

- int    cy3_WriteSPI(const unsigned char addr, const unsigned char data)

  Write one byte "data" to address "addr". Returns 0 on success or FX3_FWDWNLOAD_ERROR_CODE enum on error (see CyAPI.h).
  
- int    cy3_ReadSPI(const unsigned char addr, unsigned char* data);

  Read one byte "data" from address "addr". Returns 0 on success or FX3_FWDWNLOAD_ERROR_CODE enum on error (see CyAPI.h).