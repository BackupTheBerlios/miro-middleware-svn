// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BehaviourRepository_h
#define BehaviourRepository_h

#include "Singleton.h"
#include "Exception.h"

#include <map>
#include <string>
#include <iostream>

namespace Miro
{
  // forward declarations
  class Behaviour;
  class BehaviourRepository;

  //! Ostream operator for debug purposes.
  std::ostream&
  operator << (std::ostream& ostr, const BehaviourRepository& factory);

  //! Repository of all available Abrbiters.
  /**
   * Behaviours have to be registered at the repository before
   * a Policy can be initialized.
   *
   * @author Hans Utz
   */
  class BehaviourRepository  
  {
  public:
    //! Register an Behaviour at the repository.
    void registerBehaviour(Behaviour * _behaviour) throw(Exception);
    //! Lookup an Behaviour by name.
    Behaviour* getBehaviour(const std::string& _name) throw();

    //! Accessor to the global instance of the BehaviourRepository.
    static Singleton<BehaviourRepository> instance;

  protected:
    //! Dump the BehaviourRepository to the specified output stream.
    void printToStream(std::ostream& ostr) const;

    //! Map of the registerd Behaviours, sorted by name.
    typedef std::map<std::string, Behaviour*> BehaviourMap;
    //! Map to associate an Behaviour name with an Behaviour instance.
    BehaviourMap behaviours_;

    friend std::ostream& operator << (std::ostream&, const BehaviourRepository&);
    //! The singleton implementation in charge: calls the private ctor.
    friend class ACE_Singleton<BehaviourRepository, ACE_Recursive_Thread_Mutex>;

  private:
    //! There is only one BehaviourRepository instance.
    BehaviourRepository();
    //! Copy construction is prohibited
    BehaviourRepository(const BehaviourRepository&) {}
  };
}
#endif

