/******************************************************************************
 * Copyright (c) 2012-2019 All Rights Reserved, http://www.evocortex.com      *
 *  Evocortex GmbH                                                            *
 *  Emilienstr. 1                                                             *
 *  90489 Nuremberg                                                           *
 *  Germany                                                                   *
 *                                                                            *
 * Contributors:                                                              *
 *  Initial version for Linux 64-Bit platform supported by Fraunhofer IPA,    *
 *  http://www.ipa.fraunhofer.de                                              *
 *****************************************************************************/

#ifndef IRIMAGER_H
#define IRIMAGER_H

#include <stdlib.h>
#include "IRDeviceParams.h"
#include "IRImagerClient.h"
#include "unicode.h"
#include "irdirectsdk_defs.h"
#include "IRCommonTypes.h"
#include "pif_config/IRPifConfig.h"

#if(_WIN32 & _MSC_VER <= 1700) 
typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef signed long long   int64_t;
typedef unsigned long long uint64_t; 
#else
#include <inttypes.h>
#endif

class BaseControlDevice;
struct NewFrameBuffer;
class ImageProcessing;

namespace evo
{

#define GPSBUFFERSIZE 80
#define HEADERVERSION 1003

/**
 * @struct RawdataHeader
 * @brief Meta data structure describing camera parameters of image stream
 * @author Stefan May (Evocortex GmbH), Helmut Engelhardt (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
#pragma pack(push, 1)
struct RawdataHeader
{
  uint16_t sizeOfHeader;
  uint16_t headerVersion;
  char sdate[8];
  char stime[6];
  uint32_t serial;
  int32_t width;
  int32_t height;
  uint16_t bitCount;
  int64_t avgTimePerFrame;
  uint16_t optics;               // Optics in ° or mm
  uint16_t tempMinRange;         // minimum temprange
  uint16_t tempMaxRange;         // maximum temprange
  uint16_t hwRev;                // hardware release
  uint16_t fwRev;                // firmware release
  uint16_t gpsStringBufferSize;  // size of the GPS string buffer of each frame
  uint16_t devFrequency;         // Device Frequency
  char opticsText[8];            // Optics Text
};
#pragma pack(pop)

/**
 * Callback type for thermal frames
 * @param[in] data thermal data, convert to real temperatures with ((((float)data[i])-1000.f))/10.f
 * @param[in] w width of image
 * @param[in] h height of image
 * @param[in] meta meta data container
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
typedef void(__CALLCONV *fptrIRThermalFrame)(unsigned short* data, unsigned int w, unsigned int h, IRFrameMetadata meta, void* arg);

/**
 * Callback type for visible frames
 * @param[in] data RGB data
 * @param[in] w width of image
 * @param[in] h height of image
 * @param[in] meta meta data container
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
typedef void(__CALLCONV *fptrIRVisibleFrame)(unsigned char* data, unsigned int w, unsigned int h, IRFrameMetadata meta, void* arg);

/**
 * Callback type for visible frames
 * @param[in] data RGB data
 * @param[in] w width of image
 * @param[in] h height of image
 * @param[in] meta meta data container
 * @param[in] events list of triggered events
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
typedef void(__CALLCONV *fptrIRVisibleSnapshot)(unsigned char* data, unsigned int w, unsigned int h, IRFrameMetadata meta, const IRArray<IREventData>& events, void* arg);



/**
 * Callback type for thermal frames
 * @param[in] thermal thermal data, convert to real temperatures with ((((float)data[i])-1000.f))/10.f
 * @param[in] energy energy data
 * @param[in] w width of image
 * @param[in] h height of image
 * @param[in] meta meta data container
 * @param[in] events list of triggered events
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
typedef void(__CALLCONV *fptrIRThermalSnapshot)(unsigned short* thermal, unsigned short* energy, unsigned int w, unsigned int h, IRFrameMetadata meta, const IRArray<IREventData>& events, void* arg);

/**
 * Callback type for flag state events. A registered function is called when the flag state changes.
 * @param[in] fs flag state
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
typedef void(__CALLCONV *fptrIRFlagState)(EnumFlagState fs, void* arg);

/**
 * Callback type for processing events.
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
typedef void(__CALLCONV *fptrIRProcessEvent)(void* arg);

class Timer;

/**
 * @class IRImager
 * @brief Wrapper for PI driver and image processing library
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
class __IRDIRECTSDK_API__ IRImager
{
public:

  /**
   * Standard constructor
   */
  IRImager();

  /**
   * Destructor
   */
  ~IRImager();

  /**
   * Initializing routine, to be called after instantiation
   * @param[in] params Device parameters
   * @param[in] frequency Frame rate
   * @param[in] width Raw image width (differs from thermal image)
   * @param[in] height Raw image height (differs from thermal image)
   * @param[in] useHID Use HID to control camera. If not, a dummy delegate is being used
   * @param[in] hwRev Hardware revision
   * @param[in] fwRev Firmware revision
   */
  bool init(IRDeviceParams* params, unsigned int frequency, unsigned int width, unsigned int height, bool useHID, unsigned short hwRev=0, unsigned short fwRev=0);

  /**
   * Initialize Rawdata header with camera specific parameters
   * @param[out] header Rawdata header
   */
  void initRawdataHeader(RawdataHeader &header) const;

  /**
   * Reconnect resets internal variables and calls init method again.
   */
  bool reconnect(IRDeviceParams* params, unsigned int frequency, unsigned int width, unsigned int height, bool useHID, unsigned short hwRev=0, unsigned short fwRev=0);

  /**
   * Check existence of calibration file set. Class instance must be initialized before.
   * @return Returns NULL-pointer if no file is missing, otherwise path of missing files as comma separated list. Free returned pointer with delete[].
   */
  Tchar* checkCalibration();

  /**
   * Set flag automatic
   * @param[in] autoFlag true=enable automatic, false=disable automatic
   */
  void setAutoFlag(bool autoFlag);

  /**
   * Get flag automatic
   * @return true=automatic enabled, false=automatic disabled
   */
  bool getAutoFlag() const;

  /**
   * Set flag min interval
   * @param[in] float minimal interval in seconds
   */
  void setFlagMinInterval(float seconds);

  /**
   * Get flag min interval
   * @return minimal interval in seconds
   */
  float getFlagMinInterval() const;

  /**
   * Set flag max interval
   * @param[in] float maximal interval in seconds
   */
  void setFlagMaxInterval(float seconds);

  /**
   * Get flag max interval
   * @return maximal interval in seconds
   */
  float getFlagMaxInterval() const;

  /**
   * Set temperature range
   * @param[in] tMin Minimal temperature, e.g. -20,   0, 150
   * @param[in] tMax Maximum temperature, e.g. 100, 250, 900
   */
  bool setTempRange(int tMin, int tMax);

  /**
   * Get hardware revision
   * @return revision number
   */
  unsigned int getHWRevision() const;

  /**
   * Get firmware revision
   * @return revision number
   */
  unsigned int getFWRevision() const;

  /**
   * Get libirimager version
   */
  static const char* const getVersion();

  /**
   * Get image width of thermal channel
   * @return Image width, i.e. number of columns
   */
  unsigned int getWidth() const;

  /**
   * Get image height of thermal channel
   * @return Image height, i.e. number of rows
   */
  unsigned int getHeight() const;

  /**
   * Get image width of visible channel (if available)
   * @return Image width, i.e. number of columns
   */
  unsigned int getVisibleWidth() const;

  /**
   * Get image height of visible channel (if available)
   * @return Image height, i.e. number of rows
   */
  unsigned int getVisibleHeight() const;

  /**
   * Get width of raw format (from UVC stream)
   * @return width
   */
  unsigned int getWidthIn() const;

  /**
   * Get height of raw format (from UVC stream)
   * @return height
   */
  unsigned int getHeightIn() const;

  /**
   * Get # of bits used for temperature coding
   * @return # of bits
   */
  unsigned short getBitCount() const;

  /**
   * Get average time per frame
   * @return average time
   */
  long long getAvgTimePerFrame() const;

  /**
   * Get maximum frame rate of device
   * return frame rate (in frames/second)
   */
  float getMaxFramerate() const;

  /**
   * Get time interval between hardware frames (camera timestamp)
   * @return time interval in [s]
   */
  float getHWInterval() const;

  /**
   * Check if bispectral technology is available
   * @return bispectral technology flag
   */
  bool hasBispectralTechnology() const;

  /**
   * Get energy buffer of previously acquired frame
   * @param[out] Output buffer (needs to be allocated outside having the size of getWidth()*getHeight())
   * @return success flag (==0)
   */
  int getEnergyBuffer(unsigned short* buffer) const;

  /**
   * Get meta data container of previously acquired frame
   * @param[out] Output buffer
   * @param[in] size Size of buffer in bytes
   * @return number of copied bytes
   */
  int getMetaData(unsigned char* buffer, int size) const;

  /**
   * Get temperature table parameters
   * @params[out] t1 Temperature for 1st operating point
   * @params[out] t3 Temperature for 2nd operating point
   * @params[out] tJunct Crossover temperature of junction point between both ranges
   * @params[out] e1 Energy of 1st operating point
   * @params[out] e3 Energy of 2nd operating point
   * @params[out] eJunct Energy of tJunct
   * @params[out] lambda Central wave length of the Planck function (standard range for all imagers)
   * @params[out] lambda2 Central wave length of the second range after tJunct (e.g. for PI1M)
   * @return success flag: false is returned, if init method was not called before.
   */
  bool getTemperatureTableParameters(double* t1, double* t3, double* tJunct, short* e1, short* e3, short* eJunct, double* lambda, double* lambda2) const;

  /**
   * Set callback function to be called for new frames
   * @param[in] callback Pointer to callback function for thermal channel
   */
  void setThermalFrameCallback(fptrIRThermalFrame callback);

  /**
   * Set callback function to be called for new frames
   * @param[in] callback Pointer to callback function for visible channel
   */
  void setVisibleFrameCallback(fptrIRVisibleFrame callback);

  /**
   * Set callback function to be called for changing flag states
   * @param[in] callback Pointer to callback function for flag state events
   */
  void setFlagStateCallback(fptrIRFlagState callback);

  /**
   * Set callback function to be called for PIF or software trigger events
   * @param[in] callback Pointer to callback function for events
   */
  void setThermalFrameEventCallback(fptrIRThermalSnapshot callback);

  /**
   * Set callback function to be called for PIF or software trigger events
   * @param[in] callback Pointer to callback function for events
   */
  void setVisibleFrameEventCallback(fptrIRVisibleSnapshot callback);

  /**
   * Set callback function to be called when the process method has finalized its work.
   * @param[in] callback Pointer to callback function for exit event of process method
   */
  void setProcessExitCallback(fptrIRProcessEvent callback);

  /**
   * Set client as callback receiver
   * @param[in] client callback client
   */
  void setClient(IRImagerClient* client);

  /**
   * Get pointer to image processing chain
   * @return pointer to internally used image processing chain instance
   */
  ImageProcessing* getImageProcessingChain();

  /**
   * Process raw data
   * @param[in] buffer Raw data acquired with getFrame()
   * @param[in] arg User-defined data (will be returned with callback methods)
   */
  void process(unsigned char* buffer, void* arg=NULL);

  /**
   * Force shutter flag event manually (close/open cycle)
   * @param[in] time point of time in future in [ms], when flag should be closed
   */
  void forceFlagEvent(float time=0.f);

  /**
   * Raise a software triggered snapshot event (comparable with PIF event)
   */
  void raiseSnapshotEvent();

  /**
   * Check if shutter flag is open
   * @return flag open
   */
  bool isFlagOpen() const;

  /**
   * Get temperature of shutter flag
   * @return temperature
   */
  float getTempFlag() const;

  /**
   * Get temperature of housing
   * @return temperature
   */
  float getTempBox() const;

  /**
   * Get temperature of chip
   * @return temperature
   */
  float getTempChip() const;

  /**
   * Enable heating of bolometers
   * @param[in] enable enable flag
   */
  void enableChipHeating(bool enable);

  /**
   * Get state of bolometers heating
   * @return enable flag
   */
  bool isChipHeatingEnabled() const;

  /**
   * Set reference input of bolometer heating (limited to +20° - 55°). The chip temperature can be monitored with getTempChip().
   * @param[in] t temperature in °C
   */
  void setTempChipReference(float t);

  /**
   * Get reference input of bolometer heating
   * @return temperature of bolometers in °C
   */
  float getTempChipReference() const;

  /**
   * Set radiation properties, i.e. emissivity and transmissivity parameters
   * @param[in] emissivity emissivity of observed object [0;1]
   * @param[in] transmissivity transmissivity of observed object [0;1]
   * @param[in] tAmbient ambient temperature, setting invalid values (below -273,15 degrees) forces the library to take its own measurement values.
   */
  void setRadiationParameters(float emissivity, float transmissivity, float tAmbient=-999.f);

  /**
   * Experimental feature: Use multi-threading to accelerate process call
   * @param[in] useMultiThreading Activate/Deactivate multi-threading
   */
  void setUseMultiThreading(bool useMultiThreading);

  /**
   * Set the position of the focusmotor
   * @param[in] pos fucos motor position in %
   * @param[out] error return value; false if no focusmotor is available
   */
  bool setFocusmotorPos(float pos);

  /**
   * Get the position of the focusmotor
   * @param[out] fucos motor position in % (< 0 if no focusmotor available)
   */
  float getFocusmotorPos() const;

  /**
   * Internal method not to be used by any application
   */
  void onFlagState(unsigned int flagstate);

  /**
   * Internal method not to be used by any application!
   */
  void onThermalFrameInit(unsigned int width, unsigned int height, unsigned short bitCount, long long avgTimePerFrame);

  /**
   * Internal method not to be used by any application!
   */
  void onThermalFrame(unsigned short* buffer, IRFrameMetadata meta);

  /**
   * Internal method not to be used by any application!
   */
  void onVisibleFrameInit(unsigned int width, unsigned int height);

  /**
   * Internal method not to be used by any application!
   */
  void onVisibleFrame(unsigned char* buffer, IRFrameMetadata meta);

  /**
   * Internal method not to be used by any application!
   */
  void onSnapshotEvent(unsigned short* thermal, unsigned short* energy, unsigned char* rgb, IRFrameMetadata meta, const IRArray<IREventData>& events);

  /**
   * Internal method not to be used by any application!
   */
  void onProcessExit();

  /**
   * Serialize image
   * @param[in] filename file name
   * @param[in] preview destination buffer for preview data
   * @param[in] sizePreview size of destination buffer
   */
  //int serialize(const char* filename, const char* preview, int sizePreview);

  /**
   * Activate trace module. If the processing chain needs to be debugged, this trace functionality might help. But be careful: Tracing is not thread safe!
   * Do not use it in applications having more than one camera connected.
   * @param[in] activate activation state
   */
  void activateTrace(bool activate);

  /**
   * Print trace record (needs to be activated before)
   * @param[in] pixelID ID of pixel, which should be investigated
   */
  void printTrace(const unsigned int pixelID);
  
  /**
   * Set the clipped format position. For PI1M only position with x=0 are valid.
   * @param[in] p Upper left x and y position for clipped area
   */
	void setClippedFormatPosition(Point p);
  
  /**
   * Get the clipped format position. Returns uint max if not set previously.
   * @param[out] Current clipped format position
   */
  Point getClippedFormatPosition() const;

  /**
   * @brief Enable/Disable the flag forecast
   * 
   * @param true, if flag forecast should be enabled 
   */
  void setFlagForecast(bool value);

  /**
   * @brief Returns if flag forecast is enabled/disabled
   * 
   * @return true if flag forecast is enabled
   * @return false if flag forecast is disabled
   */
  bool getFlagForecast() const;

  /**
   * Get the first radial distortion correction factor (r^2).
   * @param[out] Current first radial distortion correction factor (r^2)
   */
  double getRadialDistortionCorrFact1() const;

  /**
   * Set the first (r^2) radial distortion correction factor (Brown's distortion model).
   * @param[in] First radial distortion correction factor (r^2)
   */
  void setRadialDistortionCorrFact1(double value);

  /**
   * Get the second radial distortion correction factor (r^4).
   * @param[out] Current second radial distortion correction factor (r^4)
   */
  double getRadialDistortionCorrFact2() const;

  /**
   * Set the second (r^4) radial distortion correction factor (Brown's distortion model). First factor also needs to be non-zero to enable radial distortion correction.
   * @param[in] Second radial distortion correction factor (r^4)
   */
  void setRadialDistortionCorrFact2(double value);

  enum class RadialDistortionCorrectionMode
  {
    Off, ///< Disable radial distortion correction
    Normal, ///< Enable radial distortion correction with cut off border
    Wide ///< Enable radial distortion correction without cut off border
  };

  /**
   * Set the radial distortion correction mode
   * @param[in] Distortion correction mode
   */
  void setRadialDistortionCorr(RadialDistortionCorrectionMode value);

  /**
   * Get the radial distortion correction mode
   * @param[out] Current distortion correction mode
   */
  RadialDistortionCorrectionMode getRadialDistortionCorr();

  /**
   * @brief Get the Pif count of digital input's
   * 
   * @return unsigned short Count of digital input's
   */
  unsigned short getPifDICount() const;

  /**
   * @brief Get the Pif count of analog input's
   * 
   * @return unsigned short Count of analog input's
   */
  unsigned short getPifAICount() const;

  /**
   * @brief Get the Pif count of digital output's
   * 
   * @return unsigned short Count of digital output's
   */
  unsigned short getPifDOCount() const;

  /**
   * @brief Get the Pif count of analog output's
   * 
   * @return unsigned short Count of analog output's
   */
  unsigned short getPifAOCount() const;

  /**
   * @brief Get the Pif Device Count
   * @return unsigned char device count
   */
  unsigned char getPifDeviceCount(bool actual) const;

  /**
   * @brief Set the Pif Analog Output
   * 
   * @param channel AO-Channel number starting with 0
   * @param value Value to be set. Value is Ampere or Voltage, depends on set AnalogMode of IRPifConfig.
   */
  void setPifAO(unsigned char channel, float value);

  /**
   * @brief Set the Pif Digital Output
   * 
   * @param channel DO-Channel number starting with 0
   * @param value Value to be set.
   */
  void setPifDO(unsigned char channel, bool value);

  /**
   * Returns the last meta data. Can be used for polling the PIF-Inputs
   */
  const IRFrameMetadata& getLastMetadata();

  /**
   * @brief Get the Pif Config object
   * 
   * @return IRPifConfig Returns the current pif config. If never set before represents the actual connected pif device.
   */
  IRPifConfig getPifConfig() const;

  /**
   * @brief Set the Pif Config
   * 
   * @param config Config with new pif configuration
   */
  void setPifConfig(IRPifConfig config);

  /**
   * @brief Currently only for Xi80. Set pif type to extern stackable pif or intern
   * 
   * @param pifType Type of pif
   * @param pifDeviceCount Count of pif devices
   * @return IRPifConfig New initialized pif config for type
   */
  IRPifConfig setPifType(IRPifType::Value pifType, unsigned char pifDeviceCount);

  /**
   * @brief Returns the current pif type
   * 
   * @return IRPifType::Value Current pif type
   */
  IRPifType::Value getPifType() const;

  /**
   * @brief Returns the number of decimal places in thermal data
   * 
   * @return Number of decimal places
   */
  short getTemprangeDecimal() const;

  /**
   * Set a reference temperature to a known reference source inside the view of the camera to improve camera accuracy 
   * @param[in] referenceTemperature Real temperature of reference source
   * @param[in] measuredTemperature Measured temperature from camera of reference source
   * @param[in] tAmbient Ambient temperature, setting invalid values (below -273,15 degrees) forces the library to take its own measurement values.
   */
  void setReferenceTemperature(float referenceTemperature, float measuredTemperature, float tAmbient = -999.f);

private:

  void freeMemory();

  void startFlag();

  void initPifConfig();

  void checkResult(long result, const char* functionName) const;

  void onAutoSkimOperation(int operation);

  inline void adaptMetadata(IRFrameMetadata& meta);

  bool                  _init;

  IRDeviceParams        _params;

  unsigned int          _widthIn;

  unsigned int          _heightIn;

  unsigned int          _widthOut;

  unsigned int          _heightOut;

  long long             _avgTimePerFrame;

  unsigned short        _bitCount;

  unsigned short*       _buffer;

  unsigned short*       _bufferEnergy;

  unsigned int          _widthOutVisible;

  unsigned int          _heightOutVisible;

  unsigned char*        _bufferVisible;

  fptrIRThermalFrame    _cbThermalFrame;

  fptrIRVisibleFrame    _cbVisibleFrame;

  fptrIRFlagState       _cbFlag;

  fptrIRThermalSnapshot _cbThermalSnapshot;

  fptrIRVisibleSnapshot _cbVisibleSnapshot;

  fptrIRProcessEvent    _cbProcessExit;

  IRImagerClient*       _client;

  float                 _maxFramerate;

  unsigned int          _frequency;

  float                 _tBox;

  float                 _tChip;

  float                 _tFlag;

  float                 _emissivity;

  float                 _transmissivity;

  Timer*                _t;

  Timer*                _tManual;

  float                 _timeToManualEvent;

  BaseControlDevice*    _udev;

  NewFrameBuffer*       _srcBuffer;

  ImageProcessing*      _ip;

  unsigned int          _instanceID;

  float                 _hwinterval;

  EnumFlagState         _eFlagstate;

  bool                  _startFlag;

  IRFrameMetadata       _lastMetaData;

  IRPifConfig           _pifConfig;

  EnumFlagState         _eFlagstatePrev;

  struct InitState {
	  enum Value { SetAutoSkimDirect, WaitAutoSkimDirect, StartFlag, WaitAndStartFlag, WaitForFlagFinished, Finished = 100 };
  };

  InitState::Value      _initState;

  unsigned short        _highPrecisionTempOffset;
};

}
#endif

/**
 * \example examples/linux/minimal/serializeRaw.cpp
 * Linux example: This minimal example demonstrates how to acquire raw data from an PI imager and serialize it to disk. Raw data can be imported to PIConnect.
 */

/**
 * \example examples/linux/oop/irimagerShowOOP.cpp
 * Linux example: This object-oriented example demonstrates how to acquire data from an PI imager and use callback methods of a derived class.
 */

/**
 * \example examples/linux/oop/IRImagerHandler.cpp
 * Linux example: This object-oriented example demonstrates how to derive a class to receive method callback's when new data is available.
 */

/**
 * \example examples/linux/opengl/irimagerShow.cpp
 * Linux example: This example demonstrates how to acquire data from an PI imager and display it with a lightweight OpenGL viewer.
 */

/**
 * \example examples/linux/opengl/irimagerTwinShow.cpp
 * Linux example: This example demonstrates how to acquire data from two PI imagers simultaneously and display both data streams with a lightweight OpenGL viewer.
 */

/**
 * \example ../../platform/windows/examples/minimal/serializeRaw.cpp
 * Windows example: This minimal example demonstrates how to acquire data from an PI imager and serialize it to disk. Raw data can be imported to PIConnect.
 */

/**
 * \example ../../platform/windows/examples/directshow/irimagerShow.cpp
 * Windows example: This example demonstrates how to acquire data from an PI imager and display it via GDI.
 */

/**
 * \example ../../platform/windows/examples/directshow/irimagerShowOOP.cpp
 * Windows example: This object-oriented example demonstrates how to acquire data from an PI imager and display it via GDI.
 */

/**
 * \example examples/generic/matlab/evo_ir_matlab_example.m
 * Cross platform example: This example shows the integration of the DirectSDK in MATLAB.
 */

/**
 * \example examples/generic/daemon/irDirectDaemon.cpp
 * Cross platform example: This example shows the implementation of a TCP streaming server application.
 */

/**
 * \example examples/linux/directbinding/direct_binding_tcp_show.cpp
 * Linux example: This example shows the usage of the C-style interface to implement a client application displaying data from a TCP streaming server application.
 */

/**
 * \example ../../platform/windows/examples/directbinding/irDirectBindingTCPShow.cpp
 * Windows example: This example shows the usage of the C-style interface to implement a client application displaying data from a TCP streaming server application.
 */

/**
 * \example examples/linux/directbinding/direct_binding_usb_show.cpp
 * Linux example: This example shows the usage of the C-style interface to access a locally attached camera via USB.
 */

/**
 * \example ../../platform/windows/examples/directbinding/irDirectBindingUSBShow.cpp
 * Windows example: This example shows the usage of the C-style interface to access a locally attached camera via USB.
 */

/**
 * Windows example: This example shows the usage of the pif interface. Consists of 2 files. \n\n
 * IRImagerHandler.cpp: This is the client and shows how to access the events and the digital/analog values.
 * \example ../../platform/windows/examples/pif/IRPifExample.cpp 
 * \include ../../platform/windows/examples/pif/IRImagerHandler.cpp
 * IRPifExample.cpp: This shows the pif initialization and configuration
 * 
 */

/**
 * \example examples/linux/pif/IRPifExample.cpp
 * Linux example: This example shows the usage of the C-style interface to access a locally attached camera via USB.
 */

/**
 * \example examples/linux/externalProbe/IRExternalProbe.cpp
 * Linux example: This example shows the usage of the external reference probe BR 20AR.
 */

/**
 * \example ../../platform/windows/examples/directshow/irExternalProbe.cpp
 * Windows example: This example shows the usage of the external reference probe BR 20AR.
 */