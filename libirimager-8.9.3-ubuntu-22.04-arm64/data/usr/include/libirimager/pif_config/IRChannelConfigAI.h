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
#include "IRChannelInputMode.h"
#include "IRChannelConfig.h"
#include "irdirectsdk_defs.h"

namespace evo {
/**
 * @struct IRChannelConfigAI
 * @brief Channel config of analog input
 * @author Helmut Engelhardt (Evocortex GmbH),
 */
struct __IRDIRECTSDK_API__ IRChannelConfigAI : IRChannelConfig
{
    /**
     * @brief Don't initialize yourself. Use IRImager::getPifConfig()!
     */ 
    IRChannelConfigAI(unsigned int id) :
     IRChannelConfig(id, IRChannelType::Analog),
     Mode(IRChannelInputMode::Manual),
     IsLowActive(false),
     Threshold(0)
    {
    }

    //Windows vector needs empty default constructor
#if  __cplusplus <= 199711L
	IRChannelConfigAI() : IRChannelConfig(-1, IRChannelType::Analog),
		Mode(IRChannelInputMode::Manual),
		IsLowActive(false),
		Threshold(0)
	{
	}
#else
	IRChannelConfigAI() : IRChannelConfigAI(-1)
	{
	}
#endif

    /**
     * @brief Defines the mode of the analog input
     * 
     */
    IRChannelInputMode::Value Mode;

    /**
     * @brief Set to true, if trigger should fire when input is false
     * 
     */
    bool IsLowActive;

    /**
     * @brief Threshold value of the trigger
     * 
     */
    float Threshold;
};

}  // namespace evo