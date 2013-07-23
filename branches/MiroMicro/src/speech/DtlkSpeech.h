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
#ifndef DtlkSpeech_h
#define DtlkSpeech_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"

#include "SpeechImpl.h"

/**
 * This class starts up the SpeechImpl registers it at the
 * naming service and runs the orb.
 */

class Speech : public Miro::Server
{
  typedef Miro::Server super;

public:
  // Initialization and Termination methods.
  /**
   * Constructor.
   *
   * It takes argc and argv as arguments to pass the command line
   * options to the orb.  
   */
 Speech(int argc, char *argv[]);

  /** 
   * Destructor. 
   *
   * Deregisters stuff from the NamingService etc.
   */
  virtual ~Speech();

  // speech interface
  SpeechImpl speechImpl;
  Miro::DtlkSpeech_var pSpeech;
};
#endif




