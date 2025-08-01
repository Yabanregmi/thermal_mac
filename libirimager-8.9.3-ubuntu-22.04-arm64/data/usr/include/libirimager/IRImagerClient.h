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

#ifndef IRIMAGERCLIENT_H
#define IRIMAGERCLIENT_H
#include <iostream>
#include <vector>
#include "IRDeviceParams.h"
#include "irdirectsdk_defs.h"
#include "IRArray.h"

namespace evo
{

/**
 * @class IRImagerClient
 * @brief Interface for specifying object-oriented frame callback methods
 * @author Stefan May (Evocortex GmbH)
 */
class __IRDIRECTSDK_API__ IRImagerClient
{

public:

  /**
   * Constructor
   */
  IRImagerClient(){};

  /**
   * Destructor
   */
  virtual ~IRImagerClient(){};

  /**
  * Callback method for raw frame events. The method is called when new data is acquired from device.
  * @param[in] data raw data
  * @param[in] size size of raw data in bytes
  */
  virtual void onRawFrame(unsigned char* data, int size) = 0;

  /**
   * Callback method for thermal frames
   * @param[in] data thermal image
   * @param[in] w width of thermal image
   * @param[in] h height of thermal image
   * @param[in] meta meta data container
   * @param[in] arg user arguments (passed to process method of IRImager class)
   */
  virtual void onThermalFrame(unsigned short* data, unsigned int w, unsigned int h, IRFrameMetadata meta, void* arg) = 0;

  /**
   * Callback method for thermal frames triggered with raising edge event on PIF digital input or software trigger
   * @param[in] thermal thermal image
   * @param[in] energy energy image
   * @param[in] w width of thermal/energy images
   * @param[in] h height of thermal/energy images
   * @param[in] meta meta data container
   * @param[in] events snapshot events
   * @param[in] arg user arguments (passed to process method of IRImager class)
   */
  virtual void onThermalFrameEvent(unsigned short* thermal, unsigned short* energy, unsigned int w, unsigned int h, IRFrameMetadata meta, const IRArray<IREventData>& events, void* arg) {};

#if __cplusplus >= 201103L || _MSC_VER >= 1800
  /**
   * Deprecated! Use virtual void onThermalFrameEvent(unsigned short* thermal, unsigned short* energy, unsigned int w, unsigned int h, IRFrameMetadata meta, const IRArray<IREventData>& events, void* arg)
   */
  virtual void onThermalFrameEvent(unsigned short* thermal, unsigned short* energy, unsigned int w, unsigned int h, IRFrameMetadata meta, std::vector<IREventData> events, void* arg) final { };
#endif

  /**
   * Callback method for visible frames
   * @param[in] data visible image
   * @param[in] w width of visible image
   * @param[in] h height of visible image
   * @param[in] meta meta data container
   * @param[in] arg user arguments (passed to process method of IRImager class)
   */
  virtual void onVisibleFrame(unsigned char* data, unsigned int w, unsigned int h, IRFrameMetadata meta, void* arg) { };

  /**
   * Callback method for visible frames triggered with raising edge event on PIF digital input or software trigger
   * @param[in] data visible image
   * @param[in] w width of visible image
   * @param[in] h height of visible image
   * @param[in] meta meta data container
   * @param[in] events snapshot events
   * @param[in] arg user arguments (passed to process method of IRImager class)
   */
  virtual void onVisibleFrameEvent(unsigned char* data, unsigned int w, unsigned int h, IRFrameMetadata meta, const IRArray<IREventData>& events, void* arg) {};

#if __cplusplus >= 201103L || _MSC_VER >= 1800
  /**
   * Deprecated! Use virtual void onVisibleFrameEvent(unsigned char* data, unsigned int w, unsigned int h, IRFrameMetadata meta, const IRArray<IREventData>& events, void* arg)
   */
  virtual void onVisibleFrameEvent(unsigned char* data, unsigned int w, unsigned int h, IRFrameMetadata meta, std::vector<IREventData> events, void* arg) final { };
#endif

  /**
   * Callback method for flag state events. The method is called when the flag state changes.
   * @param[in] flagstate current flag state
   * @param[in] arg user arguments (passed to process method of IRImager class)
   */
  virtual void onFlagStateChange(EnumFlagState flagstate, void* arg) = 0;

  /**
   * Callback method for synchronizing data. This is the very last method to be called for each raw data set.
   * @param[in] arg user arguments (passed to process method of IRImager class)
   */
  virtual void onProcessExit(void* arg) = 0;

};

}

#endif
