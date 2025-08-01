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
#include "irdirectsdk_defs.h"

namespace evo {
/**
 * @struct IRChannelConfigDI
 * @brief Channel config of digital input
 * @author Helmut Engelhardt (Evocortex GmbH),
 */
struct __IRDIRECTSDK_API__ IRChannelConfigDI : IRChannelConfig
{
    /**
     * @brief Don't initialize yourself. Use IRImager::getPifConfig()!
     */ 
    IRChannelConfigDI(unsigned int id) :
     IRChannelConfig(id, IRChannelType::Digital),
     Mode(IRChannelInputMode::Manual),
     IsLowActive(false)
    {
    }

    
    //Windows vector needs empty default constructor
#if  __cplusplus <= 199711L
	IRChannelConfigDI() : IRChannelConfig(-1, IRChannelType::Digital),
		Mode(IRChannelInputMode::Manual),
		IsLowActive(false)
	{
	}
#else
	IRChannelConfigDI() : IRChannelConfigDI(-1)
	{
	}
#endif

    /**
     * @brief Defines the mode of the digital input
     * 
     */
    IRChannelInputMode::Value Mode;

    /**
     * @brief Set to true, if trigger should fire when input is false
     * 
     */
    bool IsLowActive;
};
}  // namespace evo