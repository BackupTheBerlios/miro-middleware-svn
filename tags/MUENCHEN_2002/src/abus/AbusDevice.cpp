// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "AbusDevice.h"

// Array of abus devices
// since the bus only has an 8 bit device id, there only
// can be 256 devices. Therefor we dont use a hash table but
// hold this array for all possible devices. - it about 16k in size
Abus::DeviceDescription Abus::deviceTable[256];

