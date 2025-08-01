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
#include "IRChannelType.h"

namespace evo {
/**
 * @struct IRChannelConfigAI
 * @brief Channel config of analog input
 * @author Helmut Engelhardt (Evocortex GmbH),
 */
struct __IRDIRECTSDK_API__ IRChannelConfig
{
    /**
     * @brief Don't initialize yourself. Use IRImager::getPifConfig()!
     */ 
    IRChannelConfig(unsigned int id, IRChannelType::Value channelType) : Id(id), ChannelType(channelType)
    {
    }

    /**
     * @brief Don't initialize yourself. Use IRImager::getPifConfig()!
     */
#if  __cplusplus <= 199711L
	IRChannelConfig() : Id(-1), ChannelType(IRChannelType::None)
	{
	}
#else
	IRChannelConfig() : IRChannelConfig(-1, IRChannelType::None)
	{
	}
#endif

    /**
     * @brief Channel id
     * 
     */
    int Id;

    /**
     * @brief Type of the channel
     * 
     */
    IRChannelType::Value ChannelType;
};

}  // namespace evo