// -*- c++ -*- ///////////////////////////////////////////////////////////////
/*! \file VideoConverter.cpp
 *  \brief Image conversion routines base
 *
 * -----------------------------------------------------
 *
 * RoboCup Middle Size Team, Technische Universitaet Graz
 * "Mostly Harmless" 
 * http://www.robocup.tugraz.at
 *
 * Project: Miro::Video
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 * $Log$
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#include "VideoConverter.h"

#include <miro/Exception.h>

#include <iostream>
#include <algorithm>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
//---------------------------------------------------------------
    VideoConverter::VideoConverter()
    {
    }

//---------------------------------------------------------------
    VideoConverter::~VideoConverter()
    {
    }
};
