/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id$
 */

/*
 * This file defines a set of abstract classes that provide a generalized
 * interface for communication between a PioneerConsumer/PioneerConnection
 * and a pan-tilt-zoom camera connected to the Pioneer board. Introducing
 * these classes was necessary to make the camera type (currently either
 * Canon or Sony) configurable.
 */

#ifndef PioneerCameraMessage_h
#define PioneerCameraMessage_h

#include "miro/DevMessage.h"
#include "miro/Synch.h"

// forward declarations
namespace Psos
{
  class Message;
}

namespace Pioneer
{
  /**
   * Abstract base class for messages sent from the camera implementation
   * to the PioneerConnection. The only required fields are methods to
   * query the message's buffer and length.
   */
  class CameraMessage : public virtual Miro::DevMessage
  {
  public:
    //! Query the message length
    virtual const int length() const = 0;
    //! Obtain a read-only pointer to the message buffer
    virtual const unsigned char * buffer() const = 0;
  };

  /**
   * Base class for camera feedback received and forwarded by the
   * PioneerConsumer.
   */
  class CameraAnswer : public virtual Miro::DevMessage
  {
  public:
    //! Constructor
    CameraAnswer();

    //! Reset the message, empty the buffer
    void init();
    //! Add length bytes of data to the message buffer. Called by
    //! the PioneerConsumer whenever new data is received.
    int add(const unsigned char *buffer, int length);
    //! Query the message length
    int length() const;
    //! Obtain a read-only pointer to the message buffer
    const unsigned char * buffer() const;

    //! Mutex for message buffer
    Miro::Mutex mutex;
    //! Used to indicate availability of new data
    Miro::Condition cond;

  protected:
    //! Buffer size
    static const int MAX_SIZE = 256;
    //! Current message length
    int length_;
    //! Message buffer
    unsigned char buffer_[MAX_SIZE];
  };
}

#endif /* PioneerCameraMessage_h */
