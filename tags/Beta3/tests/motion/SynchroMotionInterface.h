// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef SynchroMotionInterface_h
#define SynchroMotionInterface_h

#include "MotionInterface.h"

#include "idl/SynchroMotionC.h"

class SynchroMotionInterface : MotionInterface
{
  typedef MotionInterface Super;

public:
  SynchroMotionInterface(Miro::SynchroMotion_ptr _motion);
  virtual ~SynchroMotionInterface();

  virtual void printOptions();
  virtual bool evalCommand(char c);

protected:
  Miro::SynchroMotion_var synchroMotion_;
};
#endif
