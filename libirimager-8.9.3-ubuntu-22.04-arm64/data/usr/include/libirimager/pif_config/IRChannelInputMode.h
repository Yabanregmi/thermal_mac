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
#include "irdirectsdk_defs.h"

namespace evo {
/**
 * @struct IRChannelInputMode
 * @brief Enum for input modes
 * @author Helmut Engelhardt (Evocortex GmbH),
 */
struct __IRDIRECTSDK_API__ IRChannelInputMode
{
    enum Value
    {
        Manual,  ///< Input is evaluated manually
        Snapshot, ///< Input triggers snapshot event
        SnapshotOnEdge ///< Input triggers snapshot event on rising/falling edge depending on IsLowActive
    };
};
}  // namespace evo