// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
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

#define ARBITER_TYPES_FACTORY(X) \
    virtual X ## Parameters * getParametersInstance() const; \
    virtual X ## Message * getMessageInstance() const

#define ARBITER_TYPES_FACTORY_IMPL(X) \
  X ## Parameters * \
  X::getParametersInstance() const \
  { \
    return new X ## Parameters(); \
  } \
  X ## Message * \
  X::getMessageInstance() const \
  { \
    return new X ## Message(); \
  }

namespace Miro
{
  class ArbiterMessage;
  class ArbiterParameters;

  //! Base class for all arbiters.
  /**
   * All behaviours send their actuatory requests to an
   * arbiter. It then has to decide what to do.
   * 
   * @author Ingmar Baetge
   * @author Hans Utz
   */
  class Arbiter 
  {
  public:
    //! Default constructor
    Arbiter();
    virtual ~Arbiter();

    //! Factory method for ArbiterParameters.
    virtual ArbiterParameters * getParametersInstance() const;
    //! Factory method for ArbiterMessage.
    virtual ArbiterMessage * getMessageInstance() const;

    //! Start arbiter.
    virtual void open();
    //! Stop arbiter.
    virtual void close();
    //! True if action pattern is started.
    bool isActive() const;

    //! Initailize arbiter
    virtual void init(const ArbiterParameters * _parms);
    //! Integrate a new arbitration message into the decission process.
    virtual void arbitrate(const ArbiterMessage& _message);
    //! Determine, what to do
    virtual void calcActivation() = 0;
    //! Name of the arbiter.
    virtual const std::string& getName() const = 0;
        
  protected:
    //! A vector for arbitration input from the behaviours.
    typedef std::vector<ArbiterMessage *> MessageVector;

    //! Lock for init / arbitrate concurrency.
    Mutex mutex_;
    //! Active flag.
    /** Initialized to false. */
    bool active_;
    //! Vector of arbitration messages.
    MessageVector message_;
    //! Pointer to the parameter set of the arbiter.
    /** This is exchanged on transitions between action patterns. */
    const ArbiterParameters * params_;
  private:
    //! Copy contruction is prohibited.
    Arbiter(const Arbiter&) {}
  };

  inline
  bool 
  Arbiter::isActive() const { 
    return active_; 
  };
}
#endif
