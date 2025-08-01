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
namespace evo {
/**
 * @struct IRChannelAnalogOutputMode
 * @brief Enum for analog output modes
 * @author Helmut Engelhardt (Evocortex GmbH),
 */
struct __IRDIRECTSDK_API__ IRChannelAnalogOutputMode
{
    enum Value
    {
        Range_0V_10V,   ///< Set output range to 0V..10V
        Range_0mA_20mA, ///< Set output range to 0mA..20mA
        Range_4mA_20mA, ///< Set output range to 4mA..20mA
    };
};
}