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
#ifndef BallDetectionEventChannel_h
#define BallDetectionEventChannel_h

#include "BallDetection.h"               // Filter super class
#include "miro/StructuredPushSupplier.h" // Helper class for event publishing

namespace Video 
{
  //! Detect a red ball
  /**
   * This is just a simple example filter. Better and faster
   * implementations are surely possible.
   */
  class BallDetectionEventChannel : public BallDetection
  {
    typedef BallDetection Super;

  public:
    //! Initializing constructor.
    BallDetectionEventChannel(Miro::ImageFormatIDL const& _inputFormat);

    //! Initialization based on the supplied parameters.
    void init(Miro::Server& _server, Video::FilterParameters const * _params);
    //! Do the work.
    void process();

  private:
    //! Helper class for event publication
    Miro::StructuredPushSupplier * supplier_;
    //! The event to be published
    CosNotification::StructuredEvent event_;
  };
}
#endif
