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
#include "IRChannelAnalogOutputMode.h"
#include "irdirectsdk_defs.h"

namespace evo {
/**
 * @struct IRChannelConfigAO
 * @brief Channel config of analog output
 * @author Helmut Engelhardt (Evocortex GmbH),
 */
struct __IRDIRECTSDK_API__ IRChannelConfigAO : IRChannelConfig
{
    /**
     * @brief Don't initialize yourself. Use IRImager::getPifConfig()!
     */ 
    IRChannelConfigAO(unsigned int id) :
     IRChannelConfig(id, IRChannelType::Analog), Mode(IRChannelOutputMode::Manual), AnalogMode(IRChannelAnalogOutputMode::Range_0mA_20mA)
    {
    }

    //Windows vector needs empty default constructor
#if  __cplusplus <= 199711L
	IRChannelConfigAO() : IRChannelConfig(-1, IRChannelType::Analog),
		Mode(IRChannelOutputMode::Manual), AnalogMode(IRChannelAnalogOutputMode::Range_0mA_20mA)
	{
	}
#else
	IRChannelConfigAO() : IRChannelConfigAO(-1)
	{
	}
#endif

    /**
     * @brief Defines the mode of the analog output
     * 
     */
    IRChannelOutputMode::Value Mode;

    /**
     * @brief Determines the analog mode. Voltag or milliampere.
     * 
     */
    IRChannelAnalogOutputMode::Value AnalogMode;
};
}  // namespace evo