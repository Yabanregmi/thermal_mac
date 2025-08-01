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

#ifndef TIMER_H__
#define TIMER_H__

#include "irdirectsdk_defs.h"

namespace evo
{

/**
 * @class Timer
 * @brief Time measurement class
 * @author Stefan May (Evocortex GmbH)
 */
class __IRDIRECTSDK_API__ Timer
{

public:

	/**
	 * Constructor
	 * @brief Reference time is taken at instantiation
	 */
	Timer();

	/**
	 * Destructor
	 */
	~Timer();

	/**
	 * Reset time measurement, i.e., set new reference
	 * @return elapsed time since last reset in [ms.µs]
	 */
	long double reset();

	/**
	 * Retrieve elapsed time in [ms.µs] without timer reset
	 * @return elapsed time since last reset in [ms.µs]
	 */
	long double getTime();

private:

	long double getTimeNow();

	long double _timeRef;

  long double _frequency;

};

} /*namespace*/

#endif /*TIMER_H__*/
