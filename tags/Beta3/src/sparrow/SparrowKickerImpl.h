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
#ifndef SparrowKickerImpl_h
#define SparrowKickerImpl_h

#include "idl/KickerS.h"
#include "SparrowBaseConnection.h"


namespace Sparrow
{
  // forward declerations


  class KickerImpl :  public virtual POA_Miro::Kicker
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    KickerImpl(BaseConnection * _connection);
    virtual ~KickerImpl();

    //-------------------------------------------------------------------------
    // Kicker interface
    //-------------------------------------------------------------------------
    void kick(const Miro::TimeIDL& time) throw();

  protected:
    BaseConnection *         connection; // encapsulating communication to hardware
  };
};

#endif




