// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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




