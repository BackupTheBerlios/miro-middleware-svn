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
#ifndef MotionInterface_h
#define MotionInterface_h

#include "idl/MotionC.h"

class MotionInterface
{
public:
  MotionInterface(Miro::Motion_ptr _motion);
  virtual ~MotionInterface();

  virtual void printOptions();
  virtual bool evalCommand(char c);

protected:
  Miro::Motion_var motion_;
};
#endif
