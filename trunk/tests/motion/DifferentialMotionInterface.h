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
#ifndef DifferentialMotionInterface_h
#define DifferentialMotionInterface_h

#include "MotionInterface.h"

#include "miro/DifferentialMotionC.h"

class DifferentialMotionInterface : MotionInterface
{
  typedef MotionInterface Super;

public:
  DifferentialMotionInterface(Miro::DifferentialMotion_ptr _motion);
  virtual ~DifferentialMotionInterface();

  virtual void printOptions();
  virtual bool evalCommand(char c);

protected:
  Miro::DifferentialMotion_var diffMotion_;
};
#endif
