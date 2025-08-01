/******************************************************************************
 * Copyright (c) 2012-2018 All Rights Reserved, http://www.evocortex.com      *
 *  Evocortex GmbH                                                            *
 *  Emilienstr. 1                                                             *
 *  90489 Nuremberg                                                           *
 *  Germany                                                                   *
 *                                                                            *
 * Contributors:                                                              *
 *  Initial version for Linux 64-Bit platform supported by Fraunhofer IPA,    *
 *  http://www.ipa.fraunhofer.de                                              *
 *****************************************************************************/

#ifndef IRDEVICE_H_
#define IRDEVICE_H_

#include "IRImagerClient.h"
#include "irdirectsdk_defs.h"

namespace evo
{

enum IRDeviceError
{
    IRIMAGER_SUCCESS      =  0,
    IRIMAGER_NODATA       = -1,
    IRIMAGER_DISCONNECTED = -2,
    IRIMAGER_NOSYNC       = -3,
    IRIMAGER_STREAMOFF    = -4,
    IRIMAGER_EAGAIN       = -5,
    IRIMAGER_EIO          = -6,
    IRIMAGER_EUNKNOWN     = -7
};

class IRDevice;
struct IRDeviceParams;

/**
 * Callback type for raw data frames
 * @param[in] data raw data
 * @param[in] len length of raw data array
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
typedef void(__CALLCONV *fptrIRRawFrame)(unsigned char* data, int len, IRDevice* dev);

/**
 * @class IRDevice
 * @brief Generic device interface
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
class __IRDIRECTSDK_API__ IRDevice
{
public:

  static IRDevice* IRCreateDevice(IRDeviceParams& params, std::string filename);

  static IRDevice* IRCreateDevice(IRDeviceParams& params);

  static IRDevice* IRCreateEthernetDevice(IRDeviceParams& params);

  /**
   * Constructor
   */
  IRDevice();

  /**
   * Destructor
   */
  virtual ~IRDevice();

  /**
   * Status of device
   * @return stream/file opened status
   */
  virtual bool isOpen() = 0;

  /**
   * Flag indicating whether device is controlled via HID
   * @return true==HID, false==else
   */
  bool controlledViaHID();

  /**
   * Get serial number of attached device
   * @return serial number
   */
  unsigned long          getSerial();

  /**
   * Get image width
   * @return image width in pixels
   */
  unsigned int           getWidth();

  /**
   * Get image height
   * @return image height in pixels
   */
  unsigned int           getHeight();

  /**
   * Get hardware revision
   * @return firmware revision
   */
  unsigned short getHwRev();

  /**
   * Get firmware revision
   * @return firmware revision
   */
  unsigned short getFwRev();

  /**
   * Get device frequency
   * @return frame rate of device [1/s]
   */
  unsigned int           getFrequency();

  /**
   * Get raw image size (includes meta data)
   * @return Number of bytes
   */
  unsigned int           getRawBufferSize();

  /**
   * Get one frame in polling mode
   * @param[out] buffer raw data
   * @param[out] timestamp timestamp of data arrival
   * @param[in] timeoutMilliseconds timeout in milliseconds, -1 to use device frequency * 2, 0 = no timeout
   * @return grabbing status
   */
  virtual IRDeviceError  getFrame(unsigned char* buffer, double* timestamp=NULL, int timeoutMilliseconds = -1) = 0;

  /**
   * Start camera stream
   * @return success==true
   */
  virtual int            startStreaming() = 0;

  /**
   * Stop camera stream
   * @return success==true
   */
  virtual int            stopStreaming() = 0;

  /**
   * Set function called when raw data is available
   */
  virtual void           setRawFrameCallback(fptrIRRawFrame cb);

  /**
  * Set callback client
  * @param[in] client pointer to instance of imager client (onRawFrame will be called)
  **/
  virtual void           setClient(IRImagerClient* client);

  /**
   * Run device, i.e., treat main thread in blocking mode. See documentation of implemented methods for more information.
   */
  virtual void           run() = 0;

  /**
   * Exit blocking run method, if previously called.
   */
  virtual void           exit() = 0;

protected:

  bool             _ctrlViaHID;

  unsigned long    _serial;

  unsigned int     _width;

  unsigned int     _height;

  unsigned short   _hwRef;

  unsigned short   _fwRef;

  unsigned long    _rawBufferSize;

  unsigned int     _frequency;

  IRImagerClient*  _client;

  fptrIRRawFrame   _cb;
};

} // namespace

#endif // IRDEVICE_H_
