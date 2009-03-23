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
#ifndef PanTiltBase_h
#define PanTiltBase_h

#include "miro/Synch.h"

namespace Miro
{

  //! Implementation of a dummy PanTilt interface.
  /**
   * This class offers a generic implementation for the PanTilt
   *
   * @author Guillem Pag�s Gassull
   */
  class  PanTiltBase
  {
  protected:
    ACE_Recursive_Thread_Mutex mutex_;
  };
}

#endif // PanTiltImpl_h
