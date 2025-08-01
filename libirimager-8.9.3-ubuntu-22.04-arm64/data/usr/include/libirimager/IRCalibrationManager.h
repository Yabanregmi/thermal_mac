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

#ifndef IRCALIBRATIONMANAGER_H
#define IRCALIBRATIONMANAGER_H

#include <fstream>
#include <string>
#include "unicode.h"
#include "IRArray.h"
#include "irdirectsdk_defs.h"

class ConfigCali;

namespace evo
{

struct IRTempRange
{
  int tMin;
  int tMax;
};

struct IROptics
{
  IROptics(std::size_t countTempRanges = 0) : 
    tempRanges(countTempRanges),
    text(1, GEN_L("\0"))
  {
  }

  int fov;
  IRArray<IRTempRange> tempRanges;
  IRArray<Tchar> text;
  double radialDistortion;
};

#if _WIN32 && !IRDIRECTSDK_STATIC
EXPIMP_TEMPLATE template class __IRDIRECTSDK_API__ IRArray<IROptics>;
#endif

/**
 * @class IRCalibrationManager
 * @brief Class for checking and downloading calibration files for IRImager devices
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 * @date 19.7.2015
 */
class __IRDIRECTSDK_API__ IRCalibrationManager
{

public:

  /**
   * Constructor
   **/
  IRCalibrationManager(const Tchar* pathCali = nullptr, const Tchar* pathFormats = nullptr, const Tchar* dirDeadPixels = nullptr);

  /**
   * Destructor
   **/
  ~IRCalibrationManager();

  /**
   * Static singleton instance accessor
   * @return singleton instance of IRCalibrationManager class
   **/
  static IRCalibrationManager* getInstance();

   /**
   * Static release function, new with T#2251
   **/
  static void releaseInstance();

  //Disable copy, as no deep copy is implemented and destructor would delete the memory for all objects
  IRCalibrationManager& operator=(const IRCalibrationManager&) = delete;
  IRCalibrationManager(const IRCalibrationManager&) = delete;
  
  /**
   * Set calibration directory
   * @param[in] dir full directory path to folder containing calibration files
   **/
  void setCalibrationDir(const Tchar* dir);

  /**
   * Set formats directory
   * @param[in] dir full directory path to folder containing the Formats.def file
   **/
  void setFormatsDir(const Tchar* dir);


  /**
   * Set dead pixel directory
   * @param[in] dir full directory path to folder containing the deadpixels file
   **/
  void setDeadPixelDir(const Tchar* dir);
  

  /**
   * Get calibration directory
   * @return full directory path to folder containing calibration files
   **/
  const Tchar* getCalibrationDir();

  /**
   * Get formats directory
   * @return full directory path to folder containing the Formats.def file
   **/
  const Tchar* getFormatsDir();
  
  /**
   * Get dead pixel directory
   * @return full directory path to folder containing dead pixel files
   **/
  const Tchar* getDeadPixelDir();

  /**
  * Check existence of calibration file set.
  * @param[in] serial Serial number of device
  * @return Returns NULL-pointer if no file is missing, otherwise path of missing files as comma separated list. Free returned pointer with delete[].
  */
  Tchar* checkCalibration(unsigned long serial);

  /**
   * Check available optics for a specific device
   * @param[in] serial serial number of device
   * @return vector of available optics (fov)
   */
  const IRArray<IROptics>* getAvailableOptics(unsigned long serial);

  /**
  * Get (concat) path and name of configuration file
  * @param[out] Path of configuration file
  * @param[in] directory Directory of configuration file
  * @param[in] maxLen Maximum character length of path parameter
  * @param[in] fileName Name of configuration file
  * @param[in] extension Extension of configuration file
  */
  static bool createConfigFilepath(Tchar* path, const Tchar* directory, short maxLen, const Tchar* fileName, const Tchar* extension);

  /**
  * Generate XML configuration for a specific device
  * @param[in] serial Serial number of device
  * @return text content of standard configuration file
  */
  IRArray<Tchar> generateConfiguration(unsigned long serial);

  /**
   * Check for Internet access to calibration files
   * @return availability flag
   */
  static bool isOnlineCalibrationRepoAccessible();

  /**
   * Download calibration files for a specific serial number
   * @param serial Serial number of device
   */
  bool downloadCalibration(unsigned long serial);

  /**
   * ONLY LINUX: Copy calibration files from local device, e.g., USB stick
   * @param[in] serial serial number of device for which calibration files are to be found
   * @param[in] srcDir source directory to search for calibration files
   * @return success of search and copy operation
   */
  bool copyCalibrationFromLocalRepo(unsigned long serial, const char* srcDir);

  /**
  * Determine serial number of attached device(s)
  * @param[in] serial Serial number (pass 0 to search for serial of an attached device, pass serial to search for a specific device).
  * @param[in] skip Skips the first n results. Use this to detect multiple devices.
  * @return success flag
  */
  static bool findSerial(unsigned long &query, unsigned int skipNResults = 0);

private:


  void init(const Tchar* caliPathDefault, const Tchar* formatsPathDefault);

  static bool downloadTarget(Tchar* listOfMissingFiles, unsigned long serial, const Tchar* targetDir);

  bool checkCaliFiles(const Tchar* CaliDirectory, int SerialNumber, Tchar* ListOfMissingFiles, int* Maxlen, const Tchar* configPattern, const Tchar* caliPattern, const Tchar* kennliniePattern, const Tchar* separator);

  Tchar* _pathCali;

  Tchar* _pathFormats;

  Tchar* _pathDeadPixels;

  Tchar* _framerateText;
  
  IRArray<IROptics> _optics;
};

}

#endif // IRCALIBRATIONMANAGER_H
