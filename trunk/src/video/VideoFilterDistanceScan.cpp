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

#include "VideoFilterDistanceScan.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"
#include "miro/Log.h"
#include "miro/Server.h"
#include "idl/DistanceScanC.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

namespace Video
{


  //---------------------------------------------------------------
  FilterDistanceScan::FilterDistanceScan(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat),
    pSupplier_(NULL)
  {
    MIRO_LOG_CTOR("Video::FilterDistanceScan");
    if (_inputFormat.palette != Miro::GREY_8)
      throw Miro::Exception("Incompatible input format for FilterDistanceScan.");
    outputFormat_.palette = Miro::GREY_8;

  }

  //---------------------------------------------------------------
  FilterDistanceScan::~FilterDistanceScan()
  {
  }

  //---------------------------------------------------------------
  void
  FilterDistanceScan::process()
  {
    
    std::cout << "Process Of Distance Scan" << std::endl;
    
    unsigned char const * src_img = inputBuffer();
    unsigned char * tgt_img = outputBuffer();



    for(unsigned int x = 0; x < inputFormat_.width; x++){
       for(unsigned int y = 0; y < inputFormat_.height; y++){

          *(tgt_img++) =  *(src_img++);

      }

    }
    
    if(pSupplier_){
      Miro::DistanceScanIDL distScanIDL;
      distScanIDL.values.length(5);
      double angle;;

      for(int i = 0; i < 5; i++){
	angle = -(M_PI / 2.0) + i*M_PI/5.0;

	distScanIDL.values[i].angle = angle;
	distScanIDL.values[i].distance = 200;
      }

      notifyEvent_.remainder_of_body <<= distScanIDL;
      pSupplier_->sendEvent(notifyEvent_);
    }



  }
  
  void FilterDistanceScan::init(Miro::Server& _server, FilterParameters const * _params){
  
     std::cout << "FilterDistanceScan" << std::endl;
     
     
     if(pSupplier_ == NULL){
        ec_ = _server.resolveName<EventChannel>("EventChannel");     
        pSupplier_ = new Miro::StructuredPushSupplier(ec_.in(), _server.namingContextName.c_str(), false);
          
     }
     
     if(pSupplier_ != NULL){      
       
       notifyEvent_.header.fixed_header.event_type.domain_name =
           CORBA::string_dup(pSupplier_->domainName().c_str());
       notifyEvent_.header.fixed_header.event_type.type_name =
          CORBA::string_dup("OmniDistance");
       notifyEvent_.header.fixed_header.event_name = CORBA::string_dup("");
       notifyEvent_.header.variable_header.length(0);   // put nothing here
       notifyEvent_.filterable_data.length(0);          // put nothing here

       CosNotification::EventTypeSeq offers;
       offers.length(1);
       offers[0] = notifyEvent_.header.fixed_header.event_type;
       pSupplier_->addOffers(offers);
       pSupplier_->connect();
       this->connect();

     }
     
    Super::init(_server, _params);

  
  }

};
