// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoFilterDistanceScan_h
#define VideoFilterDistanceScan_h

#include "VideoFilter.h"
#include "miro/StructuredPushSupplier.h"
#include <orbsvcs/CosNotifyCommC.h>

namespace Video
{

    //! Lookup table driven conversion from YUV411 to RGB
    class FilterDistanceScan : public Filter
    {
      typedef Filter Super;



    public:
        FilterDistanceScan(Miro::ImageFormatIDL const& _inputFormat);
	virtual ~FilterDistanceScan();
	
	
	virtual void process();
    protected:
        void init(Miro::Server& _server, FilterParameters const * _params);
    
        Miro::StructuredPushSupplier * pSupplier_;
	CosNotification::StructuredEvent notifyEvent_;
	CosNotifyChannelAdmin::EventChannel_var ec_;
	

    };
};

#endif // VideoFilterDistanceScan_h

