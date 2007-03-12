// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "VideoFilterService.h"

#ifdef MIRO_HAS_BTTV
#include "VideoDeviceBTTV.h"
#endif
#ifdef MIRO_HAS_METEOR
#include "VideoDeviceMeteor.h"
#endif
#ifdef MIRO_HAS_1394
#include "VideoDevice1394.h"
#endif
#ifdef MIRO_HAS_UNICAP
#include "VideoDeviceUnicap.h"
#endif
#ifdef MIRO_HAS_QUICKCAM
#include "VideoDeviceQuickCam.h"
#endif
#ifdef MIRO_HAS_PLAYER
#include "VideoDevicePlayer.h"
#endif

#include "VideoDeviceDummy.h"
#include "VideoFilterBasic.h"
#include "VideoFilterYUV422toYUV.h"
#include "VideoFilterYUV422toRGB.h"
#include "VideoFilterYUV411toRGB.h"
#include "VideoFilterYUV411toYUV.h"
#include "VideoFilterYUV420PtoRGB.h"
#include "VideoFilterOmni2Pan.h"
#include "VideoFilterRGBtoSeg.h"
#include "VideoFilterDistanceScan.h"

#include "VideoFilterRepository.h"

namespace Miro
{ 
  namespace VIP
  {
    FilterService::~FilterService()
    {}

    int
    FilterService::init()
    {
      int rc = -1;

      try {
	MIRO_LOG(LL_NOTICE, "Registering filters.");
	
	::Video::FilterRepository * repo = ::Video::FilterRepository::instance();
	repo->registerFilter< ::Video::DeviceDummy>("DeviceDummy");
	
#ifdef MIRO_HAS_METEOR
	repo->registerFilter< ::Video::DeviceMeteor>("DeviceMeteor");
#endif
#ifdef MIRO_HAS_BTTV
	repo->registerFilter< ::Video::DeviceBTTV>("DeviceBTTV");
#endif
#ifdef MIRO_HAS_1394
	repo->registerFilter< ::Video::Device1394>("Device1394");
#endif
#ifdef MIRO_HAS_UNICAP
	repo->registerFilter< ::Video::DeviceUnicap>("DeviceUnicap");
#endif
#ifdef MIRO_HAS_QUICKCAM
	repo->registerFilter< ::Video::DeviceQuickCam>("DeviceQuickCam");
#endif
#ifdef MIRO_HAS_PLAYER
	repo->registerFilter< ::Video::DevicePlayer>("DevicePlayer");
#endif

	repo->registerFilter< ::Video::FilterCopy>("FilterCopy");
	repo->registerFilter< ::Video::FilterSwap3>("FilterSwap3");
	repo->registerFilter< ::Video::FilterSwap4>("FilterSwap4");
	repo->registerFilter< ::Video::FilterFlip>("FilterFlip");
	repo->registerFilter< ::Video::FilterReverse>("FilterReverse");
	repo->registerFilter< ::Video::FilterHalfImage>("FilterHalfImage");
	repo->registerFilter< ::Video::FilterYUV420PtoRGB>("FilterYUV420PtoRGB");
	repo->registerFilter< ::Video::FilterOmni2Pan>("FilterOmni2Pan");
	repo->registerFilter< ::Video::FilterRGBtoSeg>("FilterRGBtoSeg");
	repo->registerFilter< ::Video::FilterDistanceScan>("FilterDistanceScan");
	repo->registerFilter< ::Video::FilterYUV422toRGB>("FilterYUV422toRGB");
	repo->registerFilter< ::Video::FilterYUV422toYUV>("FilterYUV422toYUV");
	repo->registerFilter< ::Video::FilterYUV411toRGB>("FilterYUV411toRGB");
	repo->registerFilter< ::Video::FilterYUV411toYUV>("FilterYUV411toYUV");
   
	MIRO_LOG(LL_NOTICE, "Registered filters.");
	rc = 0;
      }
      catch (const Miro::CException& e) {
	std::cerr << "Miro exception: " << e << std::endl;
      }
      catch (const Miro::Exception& e) {
	std::cerr << "Miro exception: " << e << std::endl;
      }
      catch (...) {
	std::cerr << "Unknown exception: " << std::endl;
      }
      return rc;
    }
    
    int
    FilterService::fini()
    {
      return 0;
    }
  }
}
