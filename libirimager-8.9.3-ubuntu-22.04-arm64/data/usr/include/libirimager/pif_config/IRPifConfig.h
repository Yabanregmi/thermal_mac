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

#pragma once
#include <vector>
#include "IRChannelConfigAI.h"
#include "IRChannelConfigAO.h"
#include "IRChannelConfigDI.h"
#include "IRChannelConfigDO.h"
#include "irdirectsdk_defs.h"
#include "IRArray.h"

namespace evo {

#if _WIN32 && !IRDIRECTSDK_STATIC
  EXPIMP_TEMPLATE template class __IRDIRECTSDK_API__ IRArray<IRChannelConfigDI>;
  EXPIMP_TEMPLATE template class __IRDIRECTSDK_API__ IRArray<IRChannelConfigAI>;
  EXPIMP_TEMPLATE template class __IRDIRECTSDK_API__ IRArray<IRChannelConfigDO>;
  EXPIMP_TEMPLATE template class __IRDIRECTSDK_API__ IRArray<IRChannelConfigAO>;
#endif

/**
 * @struct IRPifConfig
 * @brief Config for pif
 * @author Helmut Engelhardt (Evocortex GmbH)
 */
struct __IRDIRECTSDK_API__ IRPifConfig
{
  IRPifConfig(std::size_t countDIs = 0, std::size_t countAIs = 0, std::size_t countDOs = 0, std::size_t countAOs = 0)
  : ChannelsDI(countDIs), ChannelsAI(countAIs), ChannelsDO(countDOs), ChannelsAO(countAOs)
   {
    FlagOpenOutput.AnalogValueOpen = 0;
    FlagOpenOutput.AnalogValueClosed = 0;
    FlagOpenOutput.AnalogValueMoving = 0;
    FrameSyncOutput.AnalogValue = 0;

    for(std::size_t i = 0; i < countDIs; i++)
    {
      ChannelsDI[i] = IRChannelConfigDI(i);
    }

    for(std::size_t i = 0; i < countAIs; i++)
    {
      ChannelsAI[i] = IRChannelConfigAI(i);
    }
    
    for(std::size_t i = 0; i < countDOs; i++)
    {
      ChannelsDO[i] = IRChannelConfigDO(i);
    }

    for(std::size_t i = 0; i < countAOs; i++)
    {
      ChannelsAO[i] = IRChannelConfigAO(i);
    }
  }

#if __cplusplus >= 201103L || _MSC_VER >= 1800
  IRPifConfig(const evo::IRPifConfig& obj) = default;

  IRPifConfig& operator=(IRPifConfig& src)
  {
    FlagOpenOutput = src.FlagOpenOutput;
    FrameSyncOutput = src.FrameSyncOutput;
    FlagOpenInputChannel = src.FlagOpenInputChannel;
    ChannelsDI = src.ChannelsDI;
    ChannelsAI = src.ChannelsAI;
    ChannelsDO = src.ChannelsDO;
    ChannelsAO = src.ChannelsAO;

    return *this;
  }

  /**
   * @brief Move constructor. Thanks to vs2013 we have to implement this ourself...
   * @param[in] src IRPifConfig to move
   */
  IRPifConfig& operator=(IRPifConfig&& src)
  {
    FlagOpenOutput = std::move(src.FlagOpenOutput);
    FrameSyncOutput = std::move(src.FrameSyncOutput);
    FlagOpenInputChannel = std::move(src.FlagOpenInputChannel);
    ChannelsDI = std::move(src.ChannelsDI);
    ChannelsAI = std::move(src.ChannelsAI);
    ChannelsDO = std::move(src.ChannelsDO);
    ChannelsAO = std::move(src.ChannelsAO);

    return *this;
  }
#endif

  struct {
    /**
     * @brief Analog Output Voltage/Milliampere when Flag is open
     * 
     */
    float AnalogValueOpen;

    /**
     * @brief Analog Output Voltage/Milliampere when Flag is closed
     * 
     */
    float AnalogValueClosed;
    
    /**
     * @brief Analog Output Voltage/Milliampere when Flag is moving
     * 
     */
    float AnalogValueMoving;

    /**
     * @brief Output channel for flag. On digital true, when flag is open.
     * 
     */
    IRChannelConfig Channel;

  } FlagOpenOutput;

  struct {
    /**
     * @brief Analog Output Voltage/Milliampere on Frame for syncing
     * 
     */
    float AnalogValue;

    /**
     * @brief Output channel for frame sync
     * 
     */
    IRChannelConfig Channel;

  } FrameSyncOutput;


  /**
   * @brief Input channel for set flag state. When input is true, flag should be open.
   * 
   */
  IRChannelConfig FlagOpenInputChannel;

  /**
   * @brief Channel config of digital input channel's
   * 
   */
  IRArray<IRChannelConfigDI> ChannelsDI;

  /**
   * @brief Channel config of analog input channel's
   * 
   */
  IRArray<IRChannelConfigAI> ChannelsAI;

  /**
   * @brief Channel config of digital output channel's
   * 
   */
  IRArray<IRChannelConfigDO> ChannelsDO;

  /**
   * @brief Channel config of analog output channel's
   * 
   */
  IRArray<IRChannelConfigAO> ChannelsAO;
};

}  // namespace evo
