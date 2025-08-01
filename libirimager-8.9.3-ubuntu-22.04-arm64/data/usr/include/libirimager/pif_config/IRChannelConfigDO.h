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
#include "IRChannelOutputMode.h"
#include "irdirectsdk_defs.h"

namespace evo {
/**
 * @struct IRChannelConfigDO
 * @brief Channel config of digital output
 * @author Helmut Engelhardt (Evocortex GmbH),
 */
struct __IRDIRECTSDK_API__ IRChannelConfigDO : IRChannelConfig
{
    /**
     * @brief Don't initialize yourself. Use IRImager::getPifConfig()!
     */ 
    IRChannelConfigDO(unsigned int id) :
     IRChannelConfig(id, IRChannelType::Digital), Mode(IRChannelOutputMode::Manual)
    {
    }

    //Windows vector needs empty default constructor
#if  __cplusplus <= 199711L
	IRChannelConfigDO() : IRChannelConfig(-1, IRChannelType::Digital), Mode(IRChannelOutputMode::Manual)
	{
	}
#else
	IRChannelConfigDO() : IRChannelConfigDO(-1)
	{
	}
#endif

    /**
     * @brief Defines the mode of the digital output
     * 
     */
    IRChannelOutputMode::Value Mode;
};
}  // namespace evo