// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (Middleware for Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
#include "VideoFilterYUV422toYUV.h"
#include "VideoFilterYUV422toRGB.h"
#include "VideoFilterYUV411toRGB.h"
#include "VideoFilterYUV411toYUV.h"
#endif
#ifdef MIRO_HAS_QUICKCAM
#include "VideoDeviceQuickCam.h"
#include "VideoFilterYUV420PtoRGB.h"
#include "VideoFilterOmni2Pan.h"
#include "VideoFilterRGBtoSeg.h"
#endif
#include "VideoDeviceDummy.h"
#include "VideoFilterBasic.h"

#include "VideoFilterRepository.h"

namespace Miro
{ 
  namespace VIP
  {
    int
    FilterService::init()
    {
      int rc = -1;

      try {
	MIRO_LOG(LL_NOTICE, "Registered filters.");
	
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
	repo->registerFilter< ::Video::FilterYUV422toRGB>("FilterYUV422toRGB");
	repo->registerFilter< ::Video::FilterYUV422toYUV>("FilterYUV422toYUV");
	repo->registerFilter< ::Video::FilterYUV411toRGB>("FilterYUV411toRGB");
	repo->registerFilter< ::Video::FilterYUV411toYUV>("FilterYUV411toYUV");
#endif
	
#ifdef MIRO_HAS_QUICKCAM
	repo->registerFilter< ::Video::DeviceQuickCam>("DeviceQuickCam");
	repo->registerFilter< ::Video::FilterYUV420PtoRGB>("FilterYUV420PtoRGB");
	repo->registerFilter< ::Video::FilterOmni2Pan>("FilterOmni2Pan");
	repo->registerFilter< ::Video::FilterRGBtoSeg>("FilterRGBtoSeg");
#endif

	repo->registerFilter< ::Video::FilterCopy>("FilterCopy");
	repo->registerFilter< ::Video::FilterSwap3>("FilterSwap3");
	repo->registerFilter< ::Video::FilterSwap4>("FilterSwap4");
	repo->registerFilter< ::Video::FilterFlip>("FilterFlip");
	repo->registerFilter< ::Video::FilterReverse>("FilterReverse");
	repo->registerFilter< ::Video::FilterHalfImage>("FilterHalfImage");
   
	rc = 0;
      }
      catch (const Miro::CException& e) {
	std::cerr << "Miro exception: " << e << endl;
      }
      catch (const Miro::Exception& e) {
	std::cerr << "Miro exception: " << e << endl;
      }
      catch (...) {
	std::cerr << "Unknown exception: " << endl;
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
