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

    virtual void open();
    virtual void close();
    bool isActive() const;

    virtual void init(const ArbiterParameters * _parms) = 0;
    virtual void arbitrate(const ArbiterMessage& _message) = 0;
    virtual const std::string& getName() const = 0;
    
  protected:
    bool active_;
  };

  inline
  bool 
  Arbiter::isActive() const { 
    return active_; 
  };
};
#endif
