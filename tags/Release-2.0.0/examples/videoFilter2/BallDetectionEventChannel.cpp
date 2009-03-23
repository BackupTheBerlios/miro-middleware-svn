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
#include "BallDetectionEventChannel.h"
#include "BallResultC.h" 

#include "miro/VideoHelper.h" // Global helper functions.
#include "miro/Exception.h"   // Exception declarations.
#include "miro/Server.h"

namespace {
  using CosNotifyChannelAdmin::EventChannel;
  using CosNotifyChannelAdmin::EventChannel_var;
}

namespace Video 
{
  /**
   * The filter expects the input format palette to be Miro::RGB_24. Otherwise
   * a Miro::Exception is thrown.
   */
  BallDetectionEventChannel::BallDetectionEventChannel(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {}

  /**
   * Processing of the configuration file parameters.
   */
  void
  BallDetectionEventChannel::init(Miro::Server& _server, Video::FilterParameters const * _params)
  {
    // Super class initialization.
    Super::init(_server, _params);

    // Set up the interface
    try {
      // Get reference to the event channel
      CosNotifyChannelAdmin::EventChannel_var ec =
	_server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel");
      supplier_ = new Miro::StructuredPushSupplier(ec.in(), _server.namingContextName.c_str());

      // Define the event type which is published
      Miro::StructuredPushSupplier::
	initStructuredEvent(event_, supplier_->domainName(), "BallResult");

      // Register the event at the event channel
      supplier_->addOffer(event_);
    }
    catch (const CORBA::Exception& e) {
      std::cerr << "Set up event publication failed: " << std::endl;
    }
  }


  /**
   * Ball detection.
   */
  void
  BallDetectionEventChannel::process()
  {
    // Pointer to the output meta data
    Video::FilterImageParameters  * imageParams = outputBufferParameters();
    ImageParameters * output =
      dynamic_cast<ImageParameters *>(imageParams);

    Super::process();

    // Send the found ball position
    BallResult result;
    result.x = output->x;
    result.y = output->y;
    event_.remainder_of_body <<= result;
    supplier_->sendEvent(event_);
  }
}
