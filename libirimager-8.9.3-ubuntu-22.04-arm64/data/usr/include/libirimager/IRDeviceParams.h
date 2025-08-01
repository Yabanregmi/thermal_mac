/******************************************************************************
 * Copyright (c) 2012-2017 All Rights Reserved, http://www.evocortex.com      *
 *  Evocortex GmbH                                                            *
 *  Emilienstr. 1                                                             *
 *  90489 Nuremberg                                                           *
 *  Germany                                                                   *
 *                                                                            *
 * Contributors:                                                              *
 *  Initial version for Linux 64-Bit platform supported by Fraunhofer IPA,    *
 *  http://www.ipa.fraunhofer.de                                              *
 *****************************************************************************/

#pragma once

#include "unicode.h"
#include <iostream>
#include "irdirectsdk_defs.h"
#include <vector>
#include "IRArray.h"

namespace evo
{

enum EnumFlagState        { irFlagOpen, irFlagClose, irFlagOpening, irFlagClosing, irFlagError, irFlagInitializing};


/**
 * @brief Enum Input type of the event
 * 
 */
struct IREventInputType
{
  enum Value
  {
    DigitalInput, ///< Triggered by digital input
    AnalogInput,  ///< Triggered by analog input
    Software      ///< Triggered by software
  };
};

/**
 * @brief Enum Type of the event
 * 
 */
struct IREventType
{
  enum Value
  {
    Snapshot,       ///< Event source is snapshot
    SnapshotOnEdge  ///< Event source is snapshot on edge
  };
};

/**
 * @brief Enum Specifies the connected pif
 * 
 */
struct IRPifType
{
  enum Value
  {
    None,       ///< Invalid or none pif
    PI,         ///< Extern PI PIF
    PImV,       ///< Extern PI PIF with 0..10 Voltage Output
    Intern,     ///< Intern pif 
    Stackable,  ///< Extern stackable pif
    PImA,       ///< Extern PI PIF with 0..20 mA Output
  };
};


/**
 * @struct IREventData
 * @brief Structure containing event data information
 * @author Helmut Engelhardt (Evocortex GmbH)
 */
struct IREventData {
  IREventData(IREventInputType::Value inputType,  unsigned char channel, IREventType::Value eventType) :
    inputType(inputType), channel(channel), eventType(eventType)
  {}

  IREventInputType::Value inputType;  /*!< Input type of event */
  unsigned char channel;            /*!< Analog or digital input channel of event */
  IREventType::Value eventType;       /*!< Event type of event */
};

#if _WIN32 && !IRDIRECTSDK_STATIC
EXPIMP_TEMPLATE template class __IRDIRECTSDK_API__ IRArray<bool>;
EXPIMP_TEMPLATE template class __IRDIRECTSDK_API__ IRArray<float>;
#endif

/**
 * @struct IRFrameMetadata
 * @brief Structure containing meta data acquired from the PI imager
 * @author Stefan May (Evocortex GmbH), Helmut Engelhardt (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
struct __IRDIRECTSDK_API__ IRFrameMetadata
{
  IRFrameMetadata(std::size_t pifDIsSize, std::size_t pifAIsSize) : pifDIs(pifDIsSize), pifAIs(pifAIsSize)
  {
  }

  unsigned short size;      /*!< Size of this structure */
  unsigned int counter;     /*!< Consecutively numbered for each received frame */
  unsigned int counterHW;   /*!< Hardware counter received from device, multiply with value returned by IRImager::getAvgTimePerFrame() to get a hardware timestamp */
  long long timestamp;      /*!< Time stamp in UNITS (10000000 per second) */
  long long timestampMedia;
  EnumFlagState flagState;  /*!< State of shutter flag at capturing time */
  float tempChip;           /*!< Chip temperature */
  float tempFlag;           /*!< Shutter flag temperature */
  float tempBox;            /*!< Temperature inside camera housing */
  unsigned short pifIn;     /*!< Deprecated! */
  IRArray<bool> pifDIs;     /*!< List of digital input values */
  IRArray<float> pifAIs;    /*!< List of analog input voltages */
};

enum EnumOutputMode       { Energy = 1, Temperature = 2 };

/**
 * @struct IRDeviceParams
 * @brief Structure containing device parameters
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
struct IRDeviceParams
{
  unsigned long serial;                /*!< serial number */
  int fov;                             /*!< Field of view */
  Tchar* opticsText;                   /*!<  */
  Tchar* formatsPath;                  /*!< Path to Format.def file  */
  Tchar* caliPath;                     /*!< Path to calibration files */
  Tchar* deadPixelPath;                /*!< Path to dead pixel files */
  int tMin;                            /*!< Minimum of temperature range */
  int tMax;                            /*!< Maximum of temperature range */
  float framerate;                     /*!< Frame rate */
  int videoFormatIndex;                /*!< Used video format index, if multiple modes are supported by the device, e.g. PI400 format index 0: 32 Hz, 1: 80 Hz. */
  int bispectral;                      /*!< Use bi-spectral mode, if available (e.g. PI200). */
  int autoFlag;                        /*!< Use auto flag procedure. */
  float minInterval;                   /*!< Minimum interval for a flag cycle. It defines the time in which a flag cycle is forced at most once.*/
  float maxInterval;                   /*!< Maximum interval for a flag cycle. It defines the time in which a flag cycle is forced at least once. */
  int tChipMode;                       /*!< Chip heating: 0=Floating, 1=Auto, 2=Fixed value */
  float tChipFixedValue;               /*!< Fixed value for tChipMode=2 */
  float focus;                         /*!< position of focus motor in % of range [0; 100] */
  bool enableExtendedTempRange;        /*!< False=Off, True=On; Caution! Enables invalid extended temp range*/
  unsigned short bufferQueueSize;      /*!< internal buffer queue size */
  bool enableHighPrecision;            /*!< False=Off, True=On; Enables temperatures with more than 1 decimal places. Depends on used camera (IRImager::getTemprangeDecimal()).*/
  int radialDistortionCorrectionMode;  /*!< Distortion correction: 0=Off, 1=Normal, 2=Wide */
  int deviceAPI;                       /*!< 0=Auto; 1= DirectShow (only Windows); 2= v4l (only Linux); 3= libusb (only Linux); 4= libuvc (only Linux, experimental, needs libuvc to be installed); 5= Ethernet*/
  bool useExternalProbeForReferencing; /*!< True=On Temperature values of external probe BR20AR are accessible on first analog input*/
  bool enableMultiThreading;           /*!< If True process chain is split into two parallel threads (experimental)*/
  struct EthernetDevice
  {
    unsigned char deviceIpAddress[4];  /*!< Only with deviceAPI==5: IP Adress of camera*/
    unsigned short localUdpPort;       /*!< Only with deviceAPI==5: Local UDP port on which the camera sends data to*/
    bool checkUdpSenderIp;             /*!< Only with deviceAPI==5: If true, only udp data with camera ip as sender are processed*/
  } ethernetDevice;
};

void __IRDIRECTSDK_API__ IRDeviceParams_InitDefault(IRDeviceParams &params);

void __IRDIRECTSDK_API__ IRDeviceParams_Print(IRDeviceParams params);

/**
 * @class IRDeviceParamsReader
 * @brief Helper class for reading PI imager configuration files
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
class __IRDIRECTSDK_API__ IRDeviceParamsReader
{
public:

  /**
   * Static xml parsing method
   * @param[in] xmlFile path to xml configuration file
   * @param[out] params imager parameters read from xml file
   */
  static bool readXML(const Tchar* xmlFile, IRDeviceParams &params);

  /**
   * Static xml parsing method for 8-bit character path
   * @param[in] xmlFile path to xml configuration file
   * @param[out] params imager parameters read from xml file
   */
  static bool readXMLC(const char* xmlFile, IRDeviceParams &params);

private:

  /**
  * Constructor
  */
  IRDeviceParamsReader() {};

  /**
   * Destructor
   */
  ~IRDeviceParamsReader() {};
};

}
