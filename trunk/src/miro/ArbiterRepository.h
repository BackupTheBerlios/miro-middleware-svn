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
#ifndef ArbiterRepository_h
#define ArbiterRepository_h

#include "Singleton.h"
#include "Exception.h"

#include <string>
#include <map>
#include <iostream>


namespace Miro
{
  // forward declaration
  class Arbiter;
  class ArbiterRepository;

  //! Ostream operator for debug purposes.
  std::ostream&
  operator << (std::ostream& ostr, const ArbiterRepository& factory);

  //! Repository of all available Abrbiters.
  /**
   * Arbiters have to be registered at the repository before
   * a Policy can be initialized.
   *
   * @author Hans Utz
   */
  class ArbiterRepository 
  {
  public:
    //! Register an Arbiter at the repository.
    void registerArbiter(Arbiter * _arbiter) throw(Exception);
    //! Lookup an Arbiter by name.
    Arbiter * getArbiter(const std::string& _name) throw();

    //! Accessor to the global instance of the ArbiterRepository.
    static Singleton<ArbiterRepository> instance;

  protected:
    //! Dump the ArbiterRepository to the specified output stream.
    void printToStream(std::ostream& ostr) const;

    //! Map of the registerd Arbiters, sorted by name.
    typedef std::map<std::string, Arbiter *> ArbiterMap;
    //! Map to associate an Arbiter name with an Arbiter instance.
    ArbiterMap arbiters_;

    friend std::ostream& operator << (std::ostream&, const ArbiterRepository&);

  private:
    //! There is only one ArbiterRepository instance.
    ArbiterRepository();
    //! Copy construction is prohibited
    ArbiterRepository(const ArbiterRepository&) {}
    friend class Singleton<ArbiterRepository>;
  };
};
#endif
