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

#ifndef IRDAEMON_H_
#define IRDAEMON_H_

#include "IRDevice.h"
#include "irdirectsdk_defs.h"

namespace evo
{

/**
 * @brief Daemon providing IRImager data via TCP
 * @author Stefan May (Evocortex GmbH)
 */
class __IRDIRECTSDK_API__ IRDaemon
{
public:

  /**
   * Constructor
   */
  IRDaemon();

  /**
   * Destructor
   */
  ~IRDaemon();

  /**
   * Blocking run method: Instantiates camera and streaming objects. This method blocks until the exit method is called.
   * @param[in] params Camera parameters
   * @param[in] port TCP port
   * @return success==true
   */
  bool run(evo::IRDeviceParams* params, int port);

  /**
   * Notifies run method to terminate
   */
  void exit();

private:

  evo::IRDevice* _device;

};

} // namespace evo

#endif // IRDAEMON_H_
