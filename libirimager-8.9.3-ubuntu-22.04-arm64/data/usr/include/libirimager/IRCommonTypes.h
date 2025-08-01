/******************************************************************************
 * Copyright (c) 2012-2019 All Rights Reserved, http://www.evocortex.com      *
 *  Evocortex GmbH                                                            *
 *  Emilienstr. 1                                                             *
 *  90489 Nuremberg                                                            *
 *  Germany                                                                   *
 *                                                                            *
 * Contributors:                                                              *
 *  Initial version for Linux 64-Bit platform supported by Fraunhofer IPA,    *
 *  http://www.ipa.fraunhofer.de                                              *
 *****************************************************************************/
#pragma once
namespace evo
{

template <class T>
class Point_
{
  public:
	Point_()
	{}

    Point_(T x, T y) : x(x), y(y)
    {}

    T x;
    T y;
};

typedef Point_<unsigned short> Point2us;
typedef Point2us Point;

} //namespace evo