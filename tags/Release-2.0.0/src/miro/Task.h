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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#ifndef miroTask_hh
#define miroTask_hh

#include <ace/Task.h>

namespace Miro
{
  /** 
   * Wrapper for ACE_Task.
   *
   * This is just a simple wrapper to the ACE_Task template class
   * to shorten its declaration. Read the ACE documentation for details
   * of this class.
   *
   * Basically, by calling open() on an instance of this class or its
   * derivates, a single new thread is spawned which executes the
   * svc() method of this class - which you therefore have to override,
   * to get your stuff done.
   */
  class Task : public ACE_Task < ACE_MT_SYNCH >
  {
    typedef ACE_Task<ACE_MT_SYNCH> super;
    typedef Task self;

  public:
    Task();
    virtual ~Task();

    /**
     * Opens one single new thread, which enters the svc() method.
     *
     * The parameter is actually just ignored, but it is best 
     * to supply NULL as a matter of style.
     *
     * If you want to open more than one thread, overwrite this method.
     */
    virtual int open(void *);
  };
};

#endif




