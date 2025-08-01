/******************************************************************************
 * Copyright (c) 2012-2020 All Rights Reserved, http://www.evocortex.com      *
 *  Evocortex GmbH                                                            *
 *  Emilienstr. 10                                                            *
 *  90489 Nuremberg                                                           *
 *  Germany                                                                   *
 *                                                                            *
 * Contributors:                                                              *
 *  Initial version for Linux 64-Bit platform supported by Fraunhofer IPA,    *
 *  http://www.ipa.fraunhofer.de                                              *
 *****************************************************************************/

#pragma once
#include "irextras_defs.h"
#include <ImageBuilder.h>
#include <string>
#include <IRImager.h>
#include <memory>


namespace evo
{

#if _WIN32
  __IREXTRAS_EXPIMP_TEMPLATE__ template class __IREXTRAS_API__ std::unique_ptr<ImageBuilder>;
#endif

/**
 * @class IRTiffWriter
 * @brief Module to write PIX-Connect readable tiff files
 * @author Helmut Engelhardt (Evocortex GmbH)
 */
class __IREXTRAS_API__ IRTiffWriter
{
public:
    IRTiffWriter();
    virtual ~IRTiffWriter();

    /**
     * Get used image builder for palette image creation for setting additional properties.
     * @param[in] decimalPlaces If decimal places differs to decimal places of existing image builder a new image builder with given decimal places will be created and returned
     * 
     * @return Pointer to used ImageBuilder
     */
    ImageBuilder* getImageBuilder(short decimalPlaces = 1);

    /**
     * Write an optris tiff file to disk
     * @param[in] filePath File path to store the created tiff file
     * @param[in] irImager Pointer to evo::IRImager for acquiring additional data needed for PIX-Connect
     * @param[in] tempValues Temperature values for palette image creation
     * @param[in] width Width of temperature image
     * @param[in] height Height of temperature image
     * 
     * @return 0 on success. -1 on file error. -2 on writing error
     */
    int writeTiff(const char* filePath, const evo::IRImager *irImager, const unsigned short *tempValues, const int width, const int height);

private:
    /* data */
    short _decimalPlaces;
    std::unique_ptr<ImageBuilder> _imageBuilder;

    /**
     * Determines if system is little or big endian
     */
    bool isLittleEndian();
};

} //namespace evo
