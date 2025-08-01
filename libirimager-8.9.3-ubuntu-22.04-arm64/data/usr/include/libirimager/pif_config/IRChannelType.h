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
 * @struct IRChannelOutputMode
 * @brief Enum for output modes
 * @author Helmut Engelhardt (Evocortex GmbH),
 */
struct __IRDIRECTSDK_API__ IRChannelType
{
    enum Value
    {
        None,       ///< Unset channel type
        Digital,    ///< Channel is digital input/output
        Analog,     ///< Channel is analog input/output
        Temperature ///< Channel delivers temperature values in °C (BR20AR)
    };
};
}  // namespace evo