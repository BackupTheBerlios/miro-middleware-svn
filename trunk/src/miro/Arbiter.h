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
#ifndef Arbiter_h
#define Arbiter_h

#include "Synch.h"

#include <vector>
#include <string>

namespace Miro
{
  class ArbiterMessage;
  class ArbiterParameters;

  class Arbiter 
  {
  public:
    Arbiter();
    virtual ~Arbiter();

    // factory method for ArbiterParameters
    virtual ArbiterParameters * getParametersInstance();
    virtual ArbiterMessage * getMessageInstance();

    virtual void open();
    virtual void close();
    bool isActive() const;

    virtual void init(const ArbiterParameters * _parms);
    virtual void arbitrate(const ArbiterMessage& _message);
    virtual void calcActivation() = 0;
    virtual const std::string& getName() const = 0;
        
  protected:
    typedef std::vector<ArbiterMessage *> MessageVector;

    Mutex mutex_;
    bool active_;
    MessageVector message_;

    const ArbiterParameters * params_;
  };

  inline
  bool 
  Arbiter::isActive() const { 
    return active_; 
  };
};
#endif
